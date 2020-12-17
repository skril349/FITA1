<template>
  <div id="app">
    <h1>Dissenya el teu hort</h1>
    <div>
      <grid
        :center="false"
        :draggable="true"
        :sortable="true"
        :items="colors"
        :cellWidth="180"
        :cellHeight="180"
        :height="100"
        :width="100"
        @click="click"
        @sort="sort"
      >
        <template slot="cell" scope="props">
          <Icon
            :color="props.item"
            :index="props.index"
            :sort="props.sort"
            :with-button="true"
            @sort="props.sort()"
          >{{props.item.plant}}<!--{{props.sort }}--></Icon>
        </template>
      </grid>
      <b-button class="buttons" variant="primary" v-on:click="Send()">
        <b-icon-cloud-upload></b-icon-cloud-upload>Send
      </b-button>
      <br />
      <br />
      <span>{{this.positions}}</span>
    </div>
  </div>
</template>

<script>
var valor = 40;
import Icon from "@/components/Icon.vue";
import { generateRGBColors } from "../utils.js";
import axios from "axios";
import cors from "cors";
import VueAxios from "vue-axios";
var url_coordenades = "http://192.168.11.118:3000/coordinate";
//var url_coordenades = "posar aqui el servidor ngrok/coordinate";

export default {
  name: "home",
  components: {
    Icon
  },
  data() {
    let colors = generateRGBColors(valor);
    return {
      colors,
      selected: null,
      posicio: null,
      positions: [],
      valor: 40,
      finalpos: []
    };
  },
  methods: {
    click({ items, sort }) {
      let value = items.find(v => v.sort === sort);
      this.selected = value.item;
      console.log(this.selected.plant);
      this.posicio = value.sort;
      console.log(sort + 1);
    },
    Send(items, sort) {
      this.positions.length = 0;
      if (this.finalpos.length == 0) {
        for (var i = 0; i < valor; i++) {
          this.positions.push(this.colors[i].plant);
        }
      } else {
        this.positions.length = 0;
        for (var i = 0; i < valor; i++) {
          this.positions.push(this.finalpos[0][i].item.plant);
        }
      }

      console.log("Abans de Axios");
      axios
        .post(
          url_coordenades,
          {
            status: "success",
            description: this.positions
          },
          console.log("dins axios")
        )
        .then(function(response) {
          // console.log(response.config.data);
          console.log(response);
        })
        .catch(function(error) {
          console.log(error);
        });

      return this.positions;
    },

    sort(event) {
      this.finalpos.length = 0;
      this.finalpos.push(event.items);
      // console.log("sort", event.items);
    }
  }
};
</script>

<style>
body {
  background: #fafafa;
}
#app {
  font-family: "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: #2c3e50;
}
.color-header {
  position: relative;
  padding: 20px 0;
  box-sizing: border-box;
  margin-right: 2%;
  margin-left: 2%;
}
.buttons {
  margin-top: 5%;
  margin-right: 40%;
  margin-left: 40%;
}

.span {
  margin-top: 2%;
  margin-right: 40%;
  margin-left: 40%;
}
</style>
