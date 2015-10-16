var express = require('express');
var app = express();
var server = require('http').Server(app);
// var pi = require('libs/piSocket');
var pi = require('libs/ledwrap');

// Port
var port = process.env.PORT || 8080;

// Setup view
app.set('views', 'templates');
app.set('view engine', 'ejs');

// Setup static content
app.use(express.static('public'));

// Basic routes
var router = express.Router();
require('routes')(router);
app.use('/', router);

// Init web socket
require('socket')(server);

server.listen(port);
console.log('Server listen on port ' + port);

// Connect to Raspberry Pi
//pi.connect();

// Init PWM on Raspberry Pi
pi.initPWM();