//https://expressjs.com/en/4x/api.html#res

var express = require('express')
var app = express()
const port = 3000
const bodyParser = require("body-parser");
const router = express.Router();
var cors = require('cors')

var bool = false
var body;

app.use(cors())

const fs = require('fs')

var requestValue = function (req, res, next) {
    rawdata= fs.readFileSync('/home/pi/Desktop/FITA1/fita6/lectures.json')
    req.requestVal = JSON.parse(rawdata)
    next()
}

app.use(requestValue)

app.get('/', function (req, res) {
  var responseText = req.requestVal
  res.send(responseText)
})

app.use(express.json()) // for parsing application/json
app.use(express.urlencoded({ extended: true })) // for parsing application/x-www-form-urlencoded

app.post('/coordinate', function (req, res, next) {
  console.log(req.body)
  // body = res.json(req.body)
  body = req.body
  bool = true
  res.send(body)
})


app.get('/coordinate', function (req, res) {
  if(bool){
    res.send(body)
  }else{
    res.send("Nothing has been send yet")
  }
})

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
