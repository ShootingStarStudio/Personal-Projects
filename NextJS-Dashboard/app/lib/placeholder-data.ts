// This file contains placeholder data that you'll be replacing with real data in the Data Fetching chapter:
// https://nextjs.org/learn/dashboard-app/fetching-data
const users = [
  {
    name: 'User',
    email: 'user@nextmail.com',
    password: '123456',
  },
  {
    name: 'Admin',
    email: 'admin@nextmail.com',
    password: '123123',
  },
];

const games = [
  {
    id:'game1',
    name: 'RingFit Crate Crasher',
    type: 'fitness',
    sortby: 'descending',
    scoretype:'integer',
    formula: '-nil-',
  },
  {
    id:'game2',
    name: 'BigBrain Whack Match',
    type: 'cognitive',
    sortby: 'descending',
    scoretype: 'integer',
    formula: '-nil-',
  },
  {
    id:'game3',
    name: 'MarioParty Precision Gardening',
    type: 'social',
    sortby: 'ascending',
    scoretype: 'time',
    formula: '-nil-',
  },
];

const centers = [
  {
    id: 'AACAA',
    name: 'AAC@AA',
  },
  {
    id: 'AACBB',
    name: 'AAC@BB',
  },
  {
    id: 'AACCC',
    name: 'AAC@CC',
  },
  {
    id: 'SACDD',
    name: 'SAC@DD',
  },
  {
    id: 'SACEE',
    name: 'SAC@EE',
  },
  {
    id: 'SACFF',
    name: 'SAC@FF',
  },
];

const participants = [
  {
    id: centers[0].id+'01',
    name: 'Alex Tan',
    center_id: centers[0].id,
  },
  {
    id: centers[0].id+'02',
    name: 'Bob',
    center_id: centers[0].id,
  },
  {
    id: centers[1].id+'01',
    name: 'Sarah Goh',
    center_id: centers[1].id,
  },
];

const sessions = [
  {
    id: 'session1',
    name: "a",
    status: 'uncompleted',
    date: '2022-12-06',
  },
  {
    id: 'session2',
    name: "b",
    status: 'uncompleted',
    date: '2022-11-14',
  },
  {
    id: 'session3',
    name: "c",
    status: 'completed',
    date: '2022-10-29',
  },
  {
    id: 'session4',
    name: "c",
    status: 'completed',
    date: '2023-09-10',
  },
  {
    id: 'session5',
    name: "d",
    status: 'uncompleted',
    date: '2023-08-05',
  },
  {
    id: 'session6',
    name: "e",
    status: 'uncompleted',
    date: '2023-07-16',
  },
  {
    id: 'session7',
    name: "f",
    status: 'uncompleted',
    date: '2023-06-27',
  },
  {
    id: 'session8',
    name: "g",
    status: 'completed',
    date: '2023-06-09',
  },
  {
    id: 'session9',
    name: "h",
    status: 'completed',
    date: '2023-06-17',
  },
  {
    id: 'session10',
    name: "i",
    status: 'completed',
    date: '2023-06-07',
  },
];

const scores = [
  {
    session_id: sessions[0].id,
    game_id: games[0].id,
    participant_id: participants[0].id,
    score: 1,
  },
  {
    session_id: sessions[0].id,
    game_id: games[0].id,
    participant_id: participants[0].id,
    score: 10,
  },
  {
    session_id: sessions[0].id,
    game_id: games[0].id,
    participant_id: participants[0].id,
    score: 100,
  },
  {
    session_id: sessions[0].id,
    game_id: games[0].id,
    participant_id: participants[0].id,
    score: 1000,
  },
  {
    session_id: sessions[0].id,
    game_id: games[0].id,
    participant_id: participants[0].id,
    score: 10000,
  },
];

module.exports = {
  users,
  sessions,
  centers,
  participants,
  games,
  scores,
};
