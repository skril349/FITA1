<template>
  <div>
    <h1 class="color">Dissenya el teu hort</h1>
    <ol id="example-1">
      <li v-for="item in items" :key="item.message">
        <strong>{{ item.message }}</strong>
        <br />
      </li>
    </ol>

    <div class="grid-container" v-for="items in numrow" :key="items">
      <b-row v-for="item in numcol" class="item2" :key="item">
        <span>{{item}},{{items}},{{posicions(item,items)}}</span>
        <expand></expand>
      </b-row>
    </div>
    <br />
    <span>{{this.positions}}</span>
    <br />
    <div>
      <b-button variant="primary" v-on:click="addPlanta()">
        <b-icon-cloud-upload></b-icon-cloud-upload>addPlanta
      </b-button>
    </div>
    <span>{{this.$store.state.planta}}</span>
    <br />
    <div>
      <b-button variant="primary" v-on:click="Send()">
        <b-icon-cloud-upload></b-icon-cloud-upload>Send
      </b-button>
    </div>
    <br />
  </div>
</template>
<script>
import expand from "@/components/expand.vue";
import { mapState } from "vuex";
import axios from "axios";
import React from "react";

export default {
  name: "Home",
  components: {
    expand
  },
  data: function() {
    return {
      numrow: 4,
      numcol: 10,
      valuepos: 1,
      positions: [],
      valor: 0,
      items: [
        { message: "1- Selecciona la primera casella(1,1)" },
        { message: "2-Escollir quina planta volem" },
        { message: "3- prèmer el boto Add planta" },
        {
          message:
            "4- Continuar escollint plantes seguint el ordre d'esquerra a dreta"
        },
        {
          message:
            "5- Comprovar que s'estan adherint les plantes ala tercera posició de cada vector"
        },
        { message: "6- Al finalitar apretar el botó Send" }
      ]
    };
  },
  methods: {
    posicions: function(item, items) {
      if (this.valuepos <= this.numrow * this.numcol) {
        var list = [];
        list.push(item);
        list.push(items);
        this.positions.push(list);
        this.valuepos++;
      }
    },
    Send: function() {
      for (var i = 0; i < this.numrow * this.numcol; i++) {
        console.log(this.positions[i]);
        if (this.positions[i].length < 3) {
          this.positions[i].push(0);
        }
      }
      const axios = require("axios");

      axios.post("https://localhost:3000/coordinate").then(resp => {
        var text = "Hola";
        console.log(text);
      });
    },
    addPlanta() {
      var plantas = this.$store.state.planta;
      this.positions[this.valor].push(plantas);
      this.valor++;
      return this.positions;
    }
  }
};
</script>

<style>
.grid-container {
  display: grid;
  grid-template-columns: auto auto auto auto auto auto auto auto auto auto;
  grid-gap: 10px;
  background-color: #ad7600;
  padding: 10px;
}

.grid-container > div {
  background-color: rgba(248, 199, 64, 0.8);
  text-align: center;
  padding: 20px 0;
  margin-left: 1%;
  margin-right: 1%;
}

.item0 {
  background-color: rgba(248, 199, 64, 0.8);
  text-align: center;
  width: 100%;
  height: 100%;
  font-size: 50px;
}
.butons {
  background-color: rgba(248, 199, 64, 0.8);
  text-align: center;
  width: 100%;
  height: 100%;
  font-size: 50px;
}

.color {
  font-size: 50px;
  color: black;
}
</style>