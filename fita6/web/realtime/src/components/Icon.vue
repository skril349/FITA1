<template>
  <div class="icon" :style="style">
    <!-- <div v-if="withButton" class="icon-delete-btn" @mousedown="remove"></div> -->
    <slot></slot>
  </div>
</template>

<script>
export default {
  name: "Icon",
  props: {
    index: {
      type: Number
    },
    withButton: {
      type: Boolean,
      default: false
    },
    color: {
      type: Object,
      default: () => {
        return null;
      }
    }
  },
  computed: {
    brightness() {
      let { r, g, b } = this.color;
      return 0.299 * r + 0.587 * g + 0.114 * b;
    },
    style() {
      if (this.color) {
        let { r, g, b } = this.color;
        let background = `rgb(${r}, ${g}, ${b})`;
        let shadow = `rgba(${r}, ${g}, ${b}, 0.5)`;
        return {
          "background-color": background,
          "box-shadow": `0px 6px 20px ${shadow}`,
          color: this.brightness > 180 ? "#777" : "#f3f3f3",
          "margin-left": "50%",
          "margin-right": "250%"
        };
      }
      return null;
    }
  },
  methods: {
    // remove() {
    //   this.$emit("remove", {
    //     index: this.index
    //   });
    // }
  }
};
</script>

<style>
@keyframes shake {
  from {
    transform: rotate(-4deg);
  }
  to {
    transform: rotate(4deg);
  }
}
.icon {
  position: absolute;
  background-color: transparent;
  margin: 50px;
  height: 130px;
  width: 130px;
  border-radius: 10px;
  box-shadow: 0px 6px 10px rgba(0, 0, 0, 0.07);
  color: #777;
  font-weight: 900;
  font-size: 18px;
  line-height: 130px;
  text-align: center;
  transition: all 0.3s;
  cursor: pointer;
}
/* .icon .icon-delete-btn {
  display: block;
  position: absolute;
  width: 20px;
  height: 20px;
  border-radius: 15px;
  right: 6px;
  top: 6px;
  border: 1px solid rgba(255, 255, 255, 0.4);
  background: rgba(255, 255, 255, 0.2);
} */

.v-grid-item-dragging .icon {

  animation-name: shake;
  animation-duration: 0.07s;
  animation-iteration-count: infinite;
  animation-direction: alternate;
  margin-left: 20px;
  margin-right: 20px;
}
.v-grid-item-wrapper{
  left: 150px;
  height: 100px;
  width: 100px;
}

</style>