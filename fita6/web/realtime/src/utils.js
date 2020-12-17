export const randomInt = (fr, to) => {
  return Math.round(fr + Math.random() * to)
}

var j = 0;
var valor = 40;
export const randomPlant = (min, max) => {
  var plantas = ["Menta", "Enciam", "Pebrots", "Rabanitos"]
  if (j < valor/4) {
    j++
    return plantas[0]
  }
  if (valor/4<=j && j<2*valor/4) {
    j++
    return plantas[1]
  }
  if (2*valor/4<=j && j<3*valor/4) {
    j++
    return plantas[2]
  }
  if (3*valor/4<=j && j<=valor) {
    j++
    return plantas[3]
  }
}

export const generateRGBColors = (count) => {
  return Array.apply(null, new Array(count)).map((none, i) => {
    return {
      r: randomInt(0, 0),
      g: randomInt(0, 150),
      b: randomInt(0, 0),
      plant: randomPlant(0, 3)

    }
  })
}

export default {
  randomInt,
  generateRGBColors
}