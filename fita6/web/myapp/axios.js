

const axios = require('axios');
var temp =[]

axios.get('http://localhost:3000/').then(resp => {
    temp = resp.data.data.slice(-1)[0]
    console.log(temp);
});

axios.post('http://localhost:3000/coordinate').then(resp => {
    var text = "Hola"
    console.log(text);
});