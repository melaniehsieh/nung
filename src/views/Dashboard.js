import React, { Component } from "react";
import LineGraph from "../components/LineGraph";
import { db } from "../config/firebase";
import "./styles.css";

class Dashboard extends Component {
  state = {
    vib: [],
    time: [],
    idV: [],
    idT: [],
    idF: [],
    idT2: [],
    five: [],
    fvib: [],
    ftime: [],
    timerange: [],
    timerange2: [],
    thirty: [],
  };

  componentDidMount() {
    this.readVibration();
    this.readTime();
  }

  readVibration = () => {
    db.ref("vibration").on("value", (snapshot) => {
      let vib = [];
      snapshot.forEach((snap) => {
        vib.push(snap.val());
      });
      this.setState({ vib });

      let idV = [];
      let five = [];
      for (let i = 0; i < vib.length; ) {
        let sum = 0;
        for (let j = 0; j < 100; j++) {
          sum += +vib[i++] || 0;
          idV.push(Object.keys(snapshot.val())[j]);
        }
        five.push(sum / 100);
      }

      if (vib.length - 1 >= 100) {
        db.ref("five-vib").push(five[five.length - 2]);
      }

      if (idV.length - 1 >= 100) {
        idV.forEach((idV) => {
          db.ref(`/vibration/${idV}`).remove();
        });
      }

      this.setState({ idV, five });
    });

    db.ref("five-vib").on("value", (snapshot) => {
      let five = [];
      snapshot.forEach((snap) => {
        five.push(snap.val());
      });
      this.setState({ fvib: five });

      let idF = [];
      let thirty = [];
      for (let i = 0; i < five.length; ) {
        let sum = 0;
        for (let j = 0; j < 6; j++) {
          sum += +five[i++] || 0;
          idF.push(Object.keys(snapshot.val())[j]);
        }
        thirty.push(sum / 6);
      }

      if (five.length - 1 >= 6) {
        db.ref("thirty-vib").push(thirty[0]);
      }

      if (idF.length - 1 >= 6) {
        idF.forEach((idF) => {
          db.ref(`/five-vib/${idF}`).remove();
        });
      }

      this.setState({ idF, thirty });
    });
  };

  readTime = () => {
    db.ref("timestamp").on("value", (snapshot) => {
      let time = [];
      snapshot.forEach((snap) => {
        time.push(snap.val());
      });
      this.setState({ time });

      let idT = [];
      let timerange = [];
      for (let i = 0; i < time.length; ) {
        for (let j = 0; j < 100; j++) {
          timerange.push(time[i++] || 0);
          idT.push(Object.keys(snapshot.val())[j]);
        }
      }

      if (timerange.length - 1 >= 100) {
        db.ref("five-time").push(timerange[0]);
      }

      if (idT.length - 1 >= 100) {
        idT.forEach((idT) => {
          db.ref(`/timestamp/${idT}`).remove();
        });
      }

      this.setState({ idT, timerange });
    });

    db.ref("five-time").on("value", (snapshot) => {
      let ftime = [];
      snapshot.forEach((snap) => {
        ftime.push(snap.val());
      });
      this.setState({ ftime });

      let idT2 = [];
      let timerange2 = [];
      for (let i = 0; i < ftime.length; ) {
        for (let j = 0; j < 6; j++) {
          timerange2.push(ftime[i++] || 0);
          idT2.push(Object.keys(snapshot.val())[j]);
        }
      }

      if (timerange2.length - 1 >= 6) {
        db.ref("thirty-time").push(timerange2[0]);
      }

      if (idT2.length - 1 >= 6) {
        idT2.forEach((idT2) => {
          db.ref(`/five-time/${idT2}`).remove();
        });
      }

      this.setState({ idT2, timerange2 });
    });
  };

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
