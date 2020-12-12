import React, { Component } from "react";
import LineGraph from "../components/LineGraph";
import { db } from "../config/firebase";
import "./styles.css";
import CsvDownload from 'react-json-to-csv';

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
      
      this.setState({ vib });
      this.convertData();
    });
  };

  convertData = () => {
    let amount;
    let csvData = [];
    let vib = this.state.vib;
    let time = this.state.time;
    vib.length >= time.length ? amount = vib.length : amount = time.length; 
    for(let i = 0; i < amount; i++){
      let object = {"vib": "", "time": ""}
      if(vib[i] != null){
        object.vib = vib[i];
      }
      if(time[i] != null){
        object.time = time[i];
      }

      csvData.push(object)
    }

    this.setState({csvData});
  }

  readTime = () => {
    db.ref("timestamp").on("value", (snapshot) => {
      let time = [];
      snapshot.forEach((snap) => {
        time.push(snap.val());
      });

      this.setState({ time });
      this.convertData();
    });
  };


  render() {
    const { vib, time, csvData } = this.state;
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
