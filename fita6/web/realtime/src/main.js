import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import { BootstrapVue, BootstrapVueIcons } from 'bootstrap-vue';
import axios from 'axios'
import VueAxios from 'vue-axios'
import VueGridLayout from 'vue-grid-layout';
import 'bootstrap/dist/css/bootstrap.css';
import 'bootstrap-vue/dist/bootstrap-vue.css';

Vue.use(BootstrapVue)
Vue.use(BootstrapVueIcons)
Vue.use(VueAxios, axios)

Vue.config.productionTip = false

export const select = new Vue();

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
