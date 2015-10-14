var ledsColor = require('libs/leds/color');

module.exports = function (router) {
  router.get('/', function (req, res) {
    res.render('index');
  });

  router.post('/color', function (req, res) {
    ledsColor.setColor(0x556677);
    res.end('Ok');
  });
};