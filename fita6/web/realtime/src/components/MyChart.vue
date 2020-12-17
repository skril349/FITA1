//https://nagix.github.io/chartjs-plugin-streaming/
<script>
//npm install chartjs
//npm install chartjs-plugin-streaming
//npm install chartjs-plugin-datalabels
//npm install --save chart.js moment vue-chartjs

import { Line } from "vue-chartjs";
import "chartjs-plugin-streaming";
import ChartDataLabels from "chartjs-plugin-datalabels";
//var json_temp = require('/home/pi/Desktop/FITA1/fita6/lectures.json');
import axios from "axios";
var url = "http://192.168.11.118:3000/";
//var url = "posar aqui el servidor ngrok";

export default {
  extends: Line,
  data: function() {
    return {
      chart: null,
      temperatura: []
    };
  },
  methods: {},
  mounted() {
    this.renderChart(
      {
        plugins: [ChartDataLabels],
        datasets: [
          {
            label: "Temperatura",
            fill: true,
            cubicInterpolationMode: "monotone",
            borderColor: "rgb(255, 0, 0,1)",
            backgroundColor: "rgba(255, 0, 0, 0.5)",

            datalabels: {
              visible: true,
              backgroundColor: "rgba(255, 0, 0, 0.5)",
              color: "white",
              font: {
                weight: "bold"
              },
              formatter: function(value) {
                return value.y;
              },
              borderColor: "blue",
              borderRadius: 4,
              labels: {
                value: {},
                title: {
                  borderColor: "blue"
                }
              }
            },

            data: [],
            pointRadius: 8,
            pointStyle: "triangle"
          },
          {
            label: "Humitat",
            fill: true,
            cubicInterpolationMode: "monotone",
            borderColor: "rgba(25, 181, 254, 1)",
            backgroundColor: "rgba(25, 181, 254, 0.5)",

            datalabels: {
              visible: true,
              backgroundColor: "rgba(25, 181, 254, 0.5)",
              color: "white",
              font: {
                weight: "bold"
              },
              formatter: function(value) {
                return value.y;
              },
              borderColor: "blue",
              borderRadius: 4,
              labels: {
                value: {},
                title: {
                  borderColor: "blue"
                }
              }
            },

            data: [],
            pointRadius: 8,
            pointStyle: "triangle"
          },
          {
            label: "Pressió",
            fill: true,
            cubicInterpolationMode: "monotone",
            borderColor: "rgba(240, 255, 0, 1)",
            backgroundColor: "rgba(240, 255, 0, 0.5)",

            datalabels: {
              visible: true,
              backgroundColor: "rgba(240, 255, 0, 0.5)",
              color: "black",
              font: {
                weight: "bold"
              },
              formatter: function(value) {
                return value.y;
              },
              borderColor: "blue",
              borderRadius: 4,
              labels: {
                value: {},
                title: {
                  borderColor: "blue"
                }
              }
            },

            data: [],
            pointRadius: 8,
            pointStyle: "triangle"
          }
        ]
      },
      {
        scales: {
          xAxes: [
            {
              type: "time",
              scaleLabel: {
                display: true,
                labelString: "temps (s)"
              },
              time: {
                unit: "second"
              },
              realtime: {
                delay: 1000,
                onRefresh: function(chart) {
                  //console.log(json_temp.data.length)
                  chart.data.datasets[0].data.push({
                    x: new Date(),
                    y: getTemperature() //getValue(json_temp)
                  });
                  chart.data.datasets[1].data.push({
                    x: new Date(),
                    y: getHumidity()//getValue(json_temp)
                  });
                  chart.data.datasets[2].data.push({
                    x: new Date(),
                    y: getLumens()//getValue(json_temp)
                  });
                }
              }
            }
          ],
          yAxes: [
            {
              scaleLabel: {
                display: true,
                labelString: "Graus",
                fontColor: "red"
              },
              ticks: {
                beginAtZero: true,
                callback: function(value) {
                  return value + "º";
                }
              }
            },
            {
              scaleLabel: {
                display: true,
                labelString: "Percentatge",
                fontColor: "blue"
              },
              ticks: {
                beginAtZero: true,
                callback: function(value) {
                  return value * 100 + "%";
                }
              }
            }
          ]
        }
      }
    );
  }
};

function getTemperature() {
  var temp = [];
  axios.get(url).then(resp => {
    temp.push(Number(resp.data.temperatura.slice(-1)[0]));
    //console.log(typeof(temp))
  });
  //console.log(temp);
  return temp;
}

function getHumidity(){
  //return Math.floor(Math.random() * 1000) / 10 
  var humi = [];
  axios.get(url).then(resp => {
    humi.push(Number(resp.data.humitat.slice(-1)[0]));
    //console.log(typeof(temp))
  });
  //console.log(temp);
  return humi;
}

function getLumens(){
  //return Math.floor(Math.random() * 1000) / 10 
  var llumi = [];
  axios.get(url).then(resp => {
    llumi.push(Number(resp.data.pressure.slice(-1)[0]));
    //console.log(typeof(temp))
  });
  //console.log(temp);
  return llumi;
}

</script>
