import React, { Component } from "react";
import LineGraph from "../components/LineGraph";
import { db } from "../config/firebase";
import "./styles.css";

class Dashboard extends Component {
  state = {
    vib: [],
    time: [],
    csvData: [],
  };

  componentDidMount() {
    db.ref("pang").on("value", (snapshot) => {
      let data = [];
      let vib = [];
      let time = [];
      snapshot.forEach((snap) => {
        data.push(snap.val());
      });

      for (let i = 0; i < data.length; i++) {
        vib.push(data[i].vibration);
        time.push(data[i].timestamp);
      }

      this.setState({ vib, time });
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
