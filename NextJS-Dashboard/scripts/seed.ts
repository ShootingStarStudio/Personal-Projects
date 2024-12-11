const { db } = require('@vercel/postgres');
import '../app/lib/placeholder-data';
const bcrypt = require('bcrypt');
const { randomUUID } = require('crypto');

async function seedUsers(client: any) {
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;
    // Create the "users" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS users (
        id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
        name TEXT NOT NULL,
        email TEXT NOT NULL UNIQUE,
        password TEXT NOT NULL
      );
    `;

    console.log(`Created "users" table`);

    // Insert data into the "users" table
    const insertValue = await Promise.all(
      users.map(async (user: any) => {
        const hashedPassword = await bcrypt.hash(user.password, 10);
        return client.sql`
        INSERT INTO users (id, name, email, password)
        VALUES (${randomUUID()}, ${user.name}, ${user.email}, ${hashedPassword})
        ON CONFLICT (id) DO NOTHING;
      `;
      }),
    );

    console.log(`Seeded ${insertValue.length} users`);

    return {
      createTable,
      users: insertValue,
    };
  } catch (error) {
    console.error('Error seeding users:', error);
    throw error;
  }
}

async function seedSessions(client: any) {
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;

    // Create the "sessions" table if it doesn't exist
    const createTable = await client.sql`
    CREATE TABLE IF NOT EXISTS sessions (
    id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
    name TEXT NOT NULL,
    date DATE NOT NULL,
    status VARCHAR(255) NOT NULL
    );
    `;

    console.log(`Created "sessions" table`);

    // Insert data into the "sessions" table
    const insertValue = await Promise.all(
      sessions.map(
        (session: any) => client.sql`
        INSERT INTO sessions (id, name, date, status)
        VALUES (${randomUUID()}, ${session.name}, ${session.date}, ${session.status})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertValue.length} sessions`);

    return {
      createTable,
      sessions: insertValue,
    };
  } catch (error) {
    console.error('Error seeding sessions:', error);
    throw error;
  }
}

async function seedCenters(client: any) {
  try {
    // Create the "centers" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS centers (
        id VARCHAR(255) NOT NULL PRIMARY KEY,
        name TEXT NOT NULL
      );
    `;

    console.log(`Created "Centers" table`);

    // Insert data into the "centers" table
    const insertValue = await Promise.all(
      centers.map(
        (center: any) => client.sql`
        INSERT INTO centers (id, name)
        VALUES ( ${center.id}, ${center.name})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertValue.length} centers`);

    return {
      createTable,
      centers: insertValue,
    };
  } catch (error) {
    console.error('Error seeding centers:', error);
    throw error;
  }
}

//requires automation with center_id to fetch highest number id then +01 to it
//encrypt name
async function seedParticipants(client: any) {
  try {
    // Create the "participants" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS participants (
        id VARCHAR(255) NOT NULL PRIMARY KEY,
        name TEXT NOT NULL,
        center_id VARCHAR(255) NOT NULL
      );
    `;

    console.log(`Created "participants" table`);

    // Insert data into the "participants" table
    const insertValue = await Promise.all(
      participants.map(
        (participant: any) => client.sql`
        INSERT INTO participants (id, name, center_id)
        VALUES (${participant.id}, ${participant.name}, ${participant.center_id})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertValue.length} participants`);

    return {
      createTable,
      participants: insertValue,
    };
  } catch (error) {
    console.error('Error seeding participants:', error);
    throw error;
  }
}

async function seedGames(client: any) {
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;
    // Create the "games" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS games (
        id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
        name TEXT NOT NULL,
        type VARCHAR(255) NOT NULL,
        sortby VARCHAR(255) NOT NULL,
        scoretype VARCHAR(255) NOT NULL,
        formula TEXT NOT NULL
      );
    `;

    console.log(`Created "games" table`);

    // Insert data into the "games" table
    const insertValue = await Promise.all(
      games.map(
        (game: any) => client.sql`
        INSERT INTO games (id, name, type, sortby, scoretype, formula)
        VALUES (${randomUUID()}, ${game.name}, ${game.type}, ${game.sortby}, 
                ${game.scoretype}, ${game.formula})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertValue.length} games`);

    return {
      createTable,
      games: insertValue,
    };
  } catch (error) {
    console.error('Error seeding games:', error);
    throw error;
  }
}

//score value is a decimal of 8,3. 
//represents XXXXX.XXX        where you have a total of 8 digits, but 3 is behind the decimal place
//first digit up to 65, 10 if omitted
//second digit is up to 30, but not larger than first number, 0 if omitted
async function seedScore(client: any) {          //not created yet
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;

    // Create the "score" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS scores (
        id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
        session_id VARCHAR(255) NOT NULL,
        game_id VARCHAR(255) NOT NULL,
        participant_id VARCHAR(255) NOT NULL,
        score DECIMAL(8,3) NOT NULL
      );
    `;

    console.log(`Created "scores" table`);

    // Insert data into the "score" table
    const insertValue = await Promise.all(
      scores.map(
        (score: any) => client.sql`
        INSERT INTO scores (id, session_id, game_id, participant_id, score)
        VALUES (${randomUUID()}, ${score.session_id}, ${score.game_id}, 
                ${score.participant_id}, ${score.score})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertValue.length} scores`);

    return {
      createTable,
      scores: insertValue,
    };
  } catch (error) {
    console.error('Error seeding scores:', error);
    throw error;
  }
}

async function dropAll(client: any) {
  // to be in order of the seed  functions below
  await client.sql`DROP TABLE IF EXISTS users`;
  await client.sql`DROP TABLE IF EXISTS sessions`;
  await client.sql`DROP TABLE IF EXISTS centers`;
  await client.sql`DROP TABLE IF EXISTS participants`;
  await client.sql`DROP TABLE IF EXISTS games`;
  await client.sql`DROP TABLE IF EXISTS scores`;

  console.log("Dropped all known tables");
}

async function main() {
  const client = await db.connect();

  await dropAll(client);

  await seedUsers(client);
  await seedSessions(client);
  await seedCenters(client);
  await seedParticipants(client);
  await seedGames(client);
  await seedScore(client);

  await client.end();
}

main().catch((err) => {
  console.error(
    'An error occurred while attempting to seed the database:',
    err,
  );
});
