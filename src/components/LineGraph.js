import React, { Component } from "react";
import Chart from "chart.js";
import "./styles.css";

let myLineChart;

Chart.defaults.global.defaultFontFamily = "'PT Sans', sans-serif";
Chart.defaults.global.legend.display = false;

export default class LineGraph extends Component {
  chartRef = React.createRef();

  componentDidMount() {
    this.buildChart();
  }

  componentDidUpdate() {
    this.buildChart();
  }

  buildChart = () => {
    const myChartRef = this.chartRef.current.getContext("2d");
    const { vib, time } = this.props;

    if (typeof myLineChart !== "undefined") myLineChart.destroy();

    myLineChart = new Chart(myChartRef, {
      type: "line",
      data: {
        labels: time,
        datasets: [
          {
            label: "Vibration",
            data: vib,
            fill: false,
            borderColor: "#6610f2",
            pointBackgroundColor: "#6610f2",
          },
        ],
      },
      options: {},
    });
  };

  render() {
    return (
      <div className="graphContainer">
        <canvas id="myChart" ref={this.chartRef} />
      </div>
    );
  }
}
