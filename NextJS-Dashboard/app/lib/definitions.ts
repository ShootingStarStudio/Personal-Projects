// This file contains type definitions for your data.
// It describes the shape of the data, and what data type each property should accept.
// For simplicity of teaching, we're manually defining these types.

// However, these types are generated automatically if you're using an ORM such as Prisma.
export type User = {
  id: string;
  name: string;
  email: string;
  password: string;
};

export type Game = {
  id: string;
  name: string;
  type: 'fitness' | 'cognitive' | 'social';
  sortby: 'descending' | 'ascending';
  scoretype: 'float' | 'integer' | 'time';
  formula: string;
};

export type Center = {
  id: string;                       //center code/ prefix
  name: string;
};

export type Participant = {
  id: string;                       //unique but not UUID
  name: string;
  center_id: string;                 //fk for center
};

export type Score = {
  id: string;                       //UUID
  session_id: string;                //fk for session
  game_id: string;                   //fk for game
  participant_id: string;            //fk for participant
  score: number;
};

export type Session = {
  id: string;
  name: string;
  date: string;
  status: 'uncompleted' | 'completed';
};
