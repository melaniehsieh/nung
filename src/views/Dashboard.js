import React, { Component } from "react";
import LineGraph from "../components/LineGraph";
import { db } from "../config/firebase";
import "./styles.css";
import CsvDownload from "react-json-to-csv";

class Dashboard extends Component {
  state = {
    vib: [],
    time: [],
    csvData: [],
  };

  componentDidMount() {
    db.ref("data").on("value", (snapshot) => {
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
      this.convertData();
    });
  }

  convertData = () => {
    let amount;
    let csvData = [];
    let vib = this.state.vib;
    let time = this.state.time;
    vib.length >= time.length ? (amount = vib.length) : (amount = time.length);
    for (let i = 0; i < amount; i++) {
      let object = { vib: "", time: "" };
      if (vib[i] != null) {
        object.vib = vib[i];
      }
      if (time[i] != null) {
        object.time = time[i];
      }

      csvData.push(object);
    }

    this.setState({ csvData });
  };

  render() {
    const { vib, time, csvData } = this.state;
    return (
      <div className="container">
        <h1>Vibration Monitor</h1>
        <LineGraph vib={vib} time={time} />
        <CsvDownload
          data={csvData}
          filename="data.csv"
          className="downloadButton"
        >
          Download Data
        </CsvDownload>
      </div>
    );
  }
}

export default Dashboard;
