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
        for (let j = 0; j < 10; j++) {
          sum += +vib[i++] || 0;
          idV.push(Object.keys(snapshot.val())[j]);
        }
        five.push(sum / 10);
      }

      if (vib.length - 1 >= 10) {
        db.ref("five-vib").push(five[five.length - 2]);
      }

      if (idV.length - 1 >= 10) {
        idV.forEach((idV) => {
          db.ref(`/vibration/${idV}`).remove();
        });
      }

      this.setState({ idV, five });
      this.readFive();
    });
  };

  readFive = () => {
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
        for (let j = 0; j < 10; j++) {
          timerange.push(time[i++] || 0);
          idT.push(Object.keys(snapshot.val())[j]);
        }
      }

      if (timerange.length - 1 >= 10) {
        db.ref("five-time").push(timerange[0]);
      }

      if (idT.length - 1 >= 10) {
        idT.forEach((idT) => {
          db.ref(`/timestamp/${idT}`).remove();
        });
      }

      this.setState({ idT, timerange });
      this.readFiveT();
    });
  };

  readFiveT = () => {
    db.ref("five-time").on("value", (snapshot) => {
      let ftime = [];
      snapshot.forEach((snap) => {
        ftime.push(snap.val());
      });
      this.setState({ ftime });

      let idT2 = [];
      for (let i = 0; i < ftime.length; ) {
        for (let j = 0; j < 6; j++) {
          idT2.push(Object.keys(snapshot.val())[j]);
        }
      }

      let thirty = [];
      if (ftime.length - 1 >= 6) {
        ftime.forEach((thirty) => {
          db.ref("thirty-time").push(thirty[0]);
        });
      }

      if (idT2.length - 1 >= 6) {
        idT2.forEach((idT2) => {
          db.ref(`/five-time/${idT2}`).remove();
        });
      }

      this.setState({ idT2, thirty });
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
