<template>
  <div id="app">
    <h1>LECTURES EN TEMPS REAL</h1>
    <br />
    <div id="buttons">
      <b-button variant="success" v-on:click="DownloadJSON()">
        <b-icon-download></b-icon-download>JSON
      </b-button>
      <br />
      <br>
      <span id="demo"></span>
    </div>
    <div style="position: relative; height:40vh; width:40vw">
      <MyChart></MyChart>
    </div>
    <br/>	   
    <div>
      <br/>
      <br/>
<br/>
<br/>
<br/>
<br/>
<br/>

     <b-form-input type="email" placeholder="me@example.com"></b-form-input>
     <br/>
      <b-button variant="primary" v-on:click="SendEmail()">
        <b-icon-download></b-icon-download>SEND EMAIL
      </b-button>

    </div>	 
 </div>
</template>

<script>
import MyChart from "@/components/MyChart.vue";
import axios from "axios";
var temps = [];
var url = "http://192.168.11.118:3000/";
export default {
  name: "app",
  data: function() {
    return {
      selected: ""
    };
  },
  methods: {
    DownloadJSON() {
      var temperatura = [];
      axios.get(url).then(resp => {
        temperatura.push(resp.data);
        //console.log(temperatura)
        const dict = {
          temperatures: temperatura
        };
        //console.log(dict.temperatures[0]);
        const data = JSON.stringify(dict.temperatures[0], 0, 4);
        console.log(data);
        const blob = new Blob([data], { type: "text/plain" });
        const e = document.createEvent("MouseEvents"),
          a = document.createElement("a");
        a.download = "Temperatures.json";
        a.href = window.URL.createObjectURL(blob);
        a.dataset.downloadurl = ["text/json", a.download, a.href].join(":");
        e.initEvent(
          "click",
          true,
          false,
          window,
          0,
          0,
          0,
          0,
          0,
          false,
          false,
          false,
          false,
          0,
          null
        );
        a.dispatchEvent(e);
      });
    },
    Delete() {
      var txt;
      if (confirm("Vols eliminar el json?")) {
        txt = "Suprimit!";
      } else {
        txt = "Cancelat!";
      }
      document.getElementById("demo").innerHTML = txt;
    }
  },
  components: {
    MyChart
  }
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
.buttons {
  margin-bottom: 5%;
}
</style>
