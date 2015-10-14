var pi = require('libs/piSocket');

function setColor(color) {
  console.log('Try to set color ' + color);

  var r = (color & 0xFF0000) >> 16;
  var g = (color & 0x00FF00) >> 8;
  var b = (color & 0x0000FF) >> 0;

  var buffer = [];
  for (var i = 0; i < 90 * 3; i++) {
    buffer.push(r);
    buffer.push(g);
    buffer.push(b);
  }

  console.log(new Buffer(buffer));
  console.log('Buffer length is ' + buffer.length);

  pi.send(buffer);
}

module.exports = { setColor: setColor };