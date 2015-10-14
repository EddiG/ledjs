var net = require("net");
var socket = new net.Socket();
var host = '127.0.0.1';
var port = '9999';

socket.unref();

socket.on("connect", function () {
  console.log("Socket connected");
});

socket.on("close", function () {
  console.log("Socket disconnected");
});

socket.on("error", function () {
  console.log("Socket error");
  socket.destroy();
});

module.exports = {
  connect: function () {
    socket.connect({ host: host, port: port });
  },
  disconnect: function () {
    socket.end();
  },
  send: function (buffer) {
    // buffer = [0xff, 0xee, 0xdd]
    socket.write(new Buffer(buffer), function (err) {
      if (err == null)
        console.log("Data transfered");
      else
        console.log("Error on write to socket");
    });
  }
}