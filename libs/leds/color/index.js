var pi = require('libs/ledwrap');

function setColor(color) {
  console.log('Try to set color ' + color);

  var r = (color & 0xFF0000) >> 16;
  var g = (color & 0x00FF00) >> 8;
  var b = (color & 0x0000FF) >> 0;
  
  for (var i = 0; i < 90; i++) {
    pi.setPixel(i, r, g, b);
  }
  
  pi.show();
}

module.exports = { setColor: setColor };