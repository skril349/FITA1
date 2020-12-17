import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  strict:true,
  state: {
    planta: null,
    item:null,
    items:null
  }, 
  getters: {

  },

  mutations: {
    getData(state,plant){
      state.planta=plant
    }
  },
  actions: {},
  modules: {}
});
