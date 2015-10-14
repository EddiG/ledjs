var ledsColor = require('libs/leds/color');

module.exports = function (server) {
  var io = require('socket.io')(server);
  io.set('origins', 'localhost:*');

  io.on('connection', function (socket) {
    console.log('Web socket connected');

    socket.on('set color', function (data) {      
      console.log('Try to set color');      
      ledsColor.setColor(+data.color);
    });

    socket.on('error', function (err) {
      console.log('Web socket error: ' + err);
    });
  });
};