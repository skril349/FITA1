<template>
  <div id="app">
    <div>
      <video ref="video" id="video" width="740" height="580" autoplay></video>
    </div>
    <div>
      <b-button id="snap" v-on:click="capture()"><b-icon-camera></b-icon-camera> Photo</b-button>
    </div>
    <br>
    <canvas ref="canvas" id="canvas" width="740" height="580"></canvas>
    <br>
    <ul>
      <li v-for="c in captures" :key="c">
        <img v-bind:src="c" height="70" />
      </li>
    </ul>
    <b-button v-on:click="stopCamera()">Stop</b-button>
  </div>
</template>

<script>
export default {
  name: "app",
  data() {
    return {
      video: {},
      canvas: {},
      captures: []
    };
  },
  mounted() {
    this.video = this.$refs.video;
    if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
      navigator.mediaDevices.getUserMedia({ video: true }).then(stream => {
        this.video.srcObject =stream;
        this.video.play();
      });
    }
  },
  methods: {
    capture() {
      this.canvas = this.$refs.canvas;
      var context = this.canvas
        .getContext("2d")
        .drawImage(this.video, 0, 0, 640, 480);
      this.captures.push(canvas.toDataURL("image/png"));
    },
    stopCamera(){
     this.video.stop()
    }
  }
};
</script>

<style>
body {
  background-color: #f0f0f0;
}
#app {
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
#video {
  background-color: #000000;
}
#canvas {
  display: none;
}
li {
  display: inline;
  padding: 5px;
}
</style>