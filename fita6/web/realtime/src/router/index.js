import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import proves from '../views/proves.vue'
import proves2 from '../views/proves2.vue'
import camera from '../views/camera.vue'
import Grid from "vue-js-grid"
Vue.use(VueRouter)
Vue.use(Grid);


  const routes = [
  {
    path: '/home',
    name: 'Home',
    component: Home
  },
  {
    path: '/proves',
    name: 'proves',
    component: proves
  },
  {
    path: '/proves2',
    name: 'proves2',
    component: proves2
  },
  
  {
    path: '/camera',
    name: 'camera',
    component: camera
  },
  {
    path: '/',
    name: 'About',
    // route level code-splitting
    // this generates a separate chunk (about.[hash].js) for this route
    // which is lazy-loaded when the route is visited.
    component: () => import(/* webpackChunkName: "about" */ '../views/About.vue')
  }
]

const router = new VueRouter({
  routes
})

export default router
