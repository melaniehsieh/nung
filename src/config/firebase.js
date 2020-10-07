import firebase from "firebase/app";
import "firebase/database";

const firebaseConfig = {
  apiKey: "AIzaSyDmW7o6fyRz5nYuLwfAK7SmexFAk_KWLfs",
  authDomain: "project-nung.firebaseapp.com",
  databaseURL: "https://project-nung.firebaseio.com",
  projectId: "project-nung",
  storageBucket: "project-nung.appspot.com",
  messagingSenderId: "855755359705",
  appId: "1:855755359705:web:2b140eeb185033014805c8",
  measurementId: "G-BBH7J4FZWX",
};

firebase.initializeApp(firebaseConfig);
export const db = firebase.database();
