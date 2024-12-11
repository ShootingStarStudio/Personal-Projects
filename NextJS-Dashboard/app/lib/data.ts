import { sql } from '@vercel/postgres';
import {
  Participant,
  Session,
  User,
  Score,
  Center,
  Game,
} from './definitions';
import { formatDateToLocal } from './utils';
import { unstable_noStore as noStore } from 'next/cache';

export async function getUser(email: string) {
  try {
    const user = await sql`SELECT * FROM users WHERE email=${email}`;
    return user.rows[0] as User;
  } catch (error) {
    console.error('Failed to fetch user:', error);
    throw new Error('Failed to fetch user.');
  }
}

export async function fetchLatestSessions() {
  noStore();
  try {
    const data = await sql<Session>`
      SELECT * 
      FROM sessions
      ORDER BY sessions.date DESC
      LIMIT 5
      `;

    const latestSessions = data.rows.map((session) => ({
      ...session,
      date: formatDateToLocal(session.date),                //reduce array into local time text
    }));
    return latestSessions;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch the latest sessions.');
  }
}

export async function fetchLatestUncompletedSessions() {
  noStore();
  try {
    const data = await sql<Session>`
      SELECT * 
      FROM sessions
      WHERE sessions.status = 'uncompleted'
      ORDER BY sessions.date DESC
      LIMIT 5
      `;

    const latestSessions = data.rows.map((session) => ({
      ...session,
      date: formatDateToLocal(session.date),                //reduce array into local time text
    }));
    return latestSessions;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch the latest uncompleted sessions.');
  }
}

//fetch all pages of name/date/status
export async function fetchSessionsPages(query: string) {
  noStore();
  try {
    const data = await sql`SELECT COUNT(*)
    FROM sessions
    WHERE
      sessions.name ILIKE ${`%${query}%`} OR
      sessions.date::text ILIKE ${`%${query}%`} OR
      sessions.status ILIKE ${`%${query}%`}
  `;

    const totalPages = Math.ceil(Number(data.rows[0].count) / ITEMS_PER_PAGE);
    return totalPages;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch total number of sessions.');
  }
}

//fetch pages of name/date/status by time, and pagination
const ITEMS_PER_PAGE = 6;
export async function fetchFilteredSessions(
  query: string,
  currentPage: number,
) {
  noStore();
  const offset = (currentPage - 1) * ITEMS_PER_PAGE;

  try {
    const data = await sql<Session>`
      SELECT * FROM sessions
      WHERE     
        sessions.name ILIKE ${`%${query}%`} OR
        sessions.date::text ILIKE ${`%${query}%`} OR
        sessions.status ILIKE ${`%${query}%`}
      ORDER BY sessions.date DESC
      LIMIT ${ITEMS_PER_PAGE} OFFSET ${offset}
    `;

    return data.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch sessions.');
  }
}

//only fetch with exact id
export async function fetchSessionById(id: string) {
  noStore();
  try {
    const data = await sql<Session>`
      SELECT * FROM sessions
      WHERE sessions.id = ${id}
      LIMIT 1
    `;

    const session = data.rows.map((session) => ({
      ...session,
      date: formatDateToLocal(session.date),
    }));
    
    return session[0];
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch session by ID.');
  }
}

export async function fetchCardData() {           //used for the upper cards in dashboard
  noStore();
  try {
    // You can probably combine these into a single SQL query
    // However, we are intentionally splitting them to demonstrate
    // how to initialize multiple queries in parallel with JS.
    const sessionCountPromise = sql`SELECT COUNT(*) FROM sessions`;
    const participantCountPromise = sql`SELECT COUNT(*) FROM participants`;
    const sessionStatusPromise = sql`SELECT COUNT(*) FROM sessions WHERE status = 'completed'`;
        //  SUM(CASE WHEN status = 'completed' ELSE 0 END) AS "completed",
        //  SUM(CASE WHEN status = 'uncompleted' ELSE 0 END) AS "uncompleted"
        //  FROM sessions;

    const data = await Promise.all([
      sessionCountPromise,
      participantCountPromise,
      sessionStatusPromise,
    ]);

    const numberOfSessions = Number(data[0].rows[0].count ?? '0');
    const numberOfParticipants = Number(data[1].rows[0].count ?? '0');
    const totalCompletedSessions = Number(data[2].rows[0].count ?? '0');
    const totalUncompletedSessions = numberOfSessions - totalCompletedSessions;

    return {
      numberOfParticipants,
      numberOfSessions,
      totalCompletedSessions,
      totalUncompletedSessions,
    };
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch card data.');
  }
}

//fetch all pages of name/date/status
export async function fetchParticipantsPages(query: string) {
  noStore();
  try {
    const data = await sql`SELECT COUNT(*)
    FROM participants
    WHERE
      participants.id ILIKE ${`%${query}%`} OR
      participants.name ILIKE ${`%${query}%`}
  `;

    const totalPages = Math.ceil(Number(data.rows[0].count) / ITEMS_PER_PAGE);
    return totalPages;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch total number of participants.');
  }
}

//only fetch with exact id
export async function fetchParticipantById(id: string) {
  noStore();
  try {
    const data = await sql<Participant>`
      SELECT * FROM participants
      WHERE participants.id = ${id}
      LIMIT 1
    `;

    return data.rows[0];
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch participant by ID.');
  }
}

export async function fetchParticipants() {
  noStore();
  try {
    const data = await sql<Participant>`
      SELECT * FROM participants
      ORDER BY id ASC
    `;

    return data.rows;
  } catch (err) {
    console.error('Database Error:', err);
    throw new Error('Failed to fetch all participants.');
  }
}

export async function fetchFilteredParticipants(
  query: string, 
  currentPage: number,
) {
  noStore();
  const offset = (currentPage - 1) * ITEMS_PER_PAGE;
  try {
    const data = await sql<Participant>`
		SELECT * FROM participants
		WHERE
      participants.id ILIKE ${`%${query}%`} OR
		  participants.name ILIKE ${`%${query}%`}
		ORDER BY participants.id ASC
    LIMIT ${ITEMS_PER_PAGE} OFFSET ${offset}
	  `;

    return data.rows;
  } catch (err) {
    console.error('Database Error:', err);
    throw new Error('Failed to fetch participant table.');
  }
}

//fetch centers
//filter, id, participants from it
export async function fetchCenters() {
  noStore();
  try{
    const data = await sql<Center>`
    SELECT * FROM centers
    ORDER BY name ASC
    `;

    return data.rows;
  } catch (err) {
    console.error('Database Error:', err);
    throw new Error('Failed to fetch all centers.');
  }
}

export async function fetchCenterPages(query: string) {
  noStore();
  try {
    const data = await sql`SELECT COUNT(*)
    FROM centers
    WHERE
      centers.id ILIKE ${`%${query}%`} OR
      centers.name ILIKE ${`%${query}%`}
  `;

    const totalPages = Math.ceil(Number(data.rows[0].count) / ITEMS_PER_PAGE);
    return totalPages;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch total number of centers.');
  }
}

export async function fetchFilteredCenters(
  query: string, 
  currentPage: number,
  ) {
    noStore();
    const offset = (currentPage - 1) * ITEMS_PER_PAGE;
  try {
    const data = await sql<Participant>`
    SELECT * FROM centers
    WHERE
      centers.id ILIKE ${`%${query}%`} OR
      centers.name ILIKE ${`%${query}%`}
    ORDER BY centers.id ASC
    LIMIT ${ITEMS_PER_PAGE} OFFSET ${offset}
    `;

    return data.rows;
  } catch (err) {
    console.error('Database Error:', err);
    throw new Error('Failed to fetch center table.');
  }
}

//only fetch with exact id
export async function fetchCenterById(id: string) {
  noStore();
  try {
    const data = await sql<Center>`
      SELECT * FROM centers
      WHERE centers.id = ${id}
      LIMIT 1
    `;

    return data.rows[0];
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch center by ID.');
  }
}

export async function fetchCenterPartipants(id: string) {
  noStore();
  try {
    const data = await sql<Participant>`
      SELECT * FROM participants
      WHERE participants.center_id = ${id}
      ORDER BY participants.id ASC
    `;

    return data.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch center by ID.');
  }
}

//fetch games
export async function fetchGames() {
  noStore();
  try{
    const data = await sql<Game>`
    SELECT * FROM games
    GROUP BY type
    ORDER BY name ASC
   `;
   
   return data.rows;
 } catch (err) {
   console.error('Database Error:', err);
   throw new Error('Failed to fetch all games.');
  }
}

export async function fetchGameById(id: string) {
  noStore();
  try {
    const data = await sql<Game>`
      SELECT * FROM games
      WHERE games.id = ${id}
      LIMIT 1
    `;

    return data.rows[0];
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch game by ID.');
  }
}

export async function fetchFilteredGames(
  query: string,
  currentPage: number,
) {
  noStore();
  const offset = (currentPage - 1) * ITEMS_PER_PAGE;

  try {
    const games = await sql<Game>`
      SELECT * FROM games
      WHERE     
        games.name ILIKE ${`%${query}%`} OR
        games.type ILIKE ${`%${query}%`}
      ORDER BY games.type ASC
      LIMIT ${ITEMS_PER_PAGE} OFFSET ${offset}
    `;

    return games.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch sessions.');
  }
}




//no need to filter or search, display all games as cards
//fetch filtered score
//probably scoreboard type


export async function fetchScore() {
  // Add noStore() here to prevent the response from being cached.
  // This is equivalent to in fetch(..., {cache: 'no-store'}).
  noStore();

  try {
    // Artificially delay a response for demo purposes.
    // Don't do this in production :)

    console.log('Fetching score data...');
    await new Promise((resolve) => setTimeout(resolve, 3000));

    const data = await sql<Score>`SELECT * FROM scores`;

    console.log('Data fetch completed after 3 seconds.');

    return data.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch score data.');
  }
}

//not completed yet
export async function fetchFilteredScore(
  session_id: string
  ) {
  noStore();

  try {
    console.log('Fetching score data...');
    await new Promise((resolve) => setTimeout(resolve, 3000));

    const data = await sql<Score>`SELECT * FROM scores`;

    console.log('Data fetch completed after 3 seconds.');

    return data.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch score data.');
  }
}