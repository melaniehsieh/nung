import React, { Component } from "react";
import LineGraph from "../components/LineGraph";
import { db } from "../config/firebase";
import "./styles.css";

class Dashboard extends Component {
  state = {
    vib: [],
    time: [],
  };

  componentDidMount() {
    db.ref("vibration").on("value", (snapshot) => {
      let vib = [];
      snapshot.forEach((snap) => {
        vib.push(snap.val());
      });
      this.setState({ vib });
    });
    db.ref("timestamp").on("value", (snapshot) => {
      let time = [];
      snapshot.forEach((snap) => {
        time.push(snap.val());
      });
      this.setState({ time });
    });
  }

  render() {
    const { vib, time } = this.state;
    return (
      <div className="container">
        <h1>Vibration Monitor</h1>
        <LineGraph vib={vib} time={time} />
      </div>
    );
  }
}

export default Dashboard;
