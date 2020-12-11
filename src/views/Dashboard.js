import React, { Component } from "react";
import LineGraph from "../components/LineGraph";
import { db } from "../config/firebase";
import "./styles.css";
import CsvDownload from "react-json-to-csv";

class Dashboard extends Component {
  state = {
    vib: [],
    time: [],
    csvData: []
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
      let csv = this.state.csvData;
      csv.push({"vib": vib})
      this.setState({ vib, csv });
    });
  };

  readTime = () => {
    db.ref("timestamp").on("value", (snapshot) => {
      let time = [];
      snapshot.forEach((snap) => {
        time.push(snap.val());
      });
      let csv = this.state.csvData;
      csv.push({"time": time})
      this.setState({ time, csv });
    });
  };

  render() {
    const { vib, time, data } = this.state;
    console.log(data);
    return (
      <div className="container">
        <CsvDownload data={csvData} filename="csv_data.csv" className="downloadButton">Download Data</CsvDownload>
        <h1>Vibration Monitor</h1>
        <LineGraph vib={vib} time={time} />
      </div>
    );
  }
}

export default Dashboard;
