<template >
  <div id="app">

    <div id="nav">
      <!--<router-link to="/Proves">About </router-link>| --> 
      <router-link to="/">Gràfiques </router-link> 
      <!--<router-link to="/about">Informació </router-link>| -->
      <!-- <router-link to="/Proves2">Proves2 </router-link>| -->
      <!--<router-link to="/camera">Camera</router-link>-->
    </div>
    <router-view />
  </div>
</template>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

#nav {
  padding: 40px;
}

#nav a {
  font-weight: bold;
  color: #2c3e50;
}

#nav a.router-link-exact-active {
  color: #42b983;
}
</style>
