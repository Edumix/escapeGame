var port = 5000;
var path = require('path');
var express = require('express');
var app = express();

var cookieParser = require('cookie-parser')

var server = require('http').Server(app); //
var io = require('socket.io')(server);

var bdd_feathers = require('./data/feathers.js');

var connect = require('connect');

//---------------------------------------------------------------------------

		var dataForTemplate  = {
			message : "",
			feathers : {}
		};

//---------------------------------------------------------------------------


var session = require("express-session")({
    secret: "my-secret",
    resave: true,
    saveUninitialized: true
});
var sharedsession = require("express-socket.io-session");

app.use(session);

io.use(sharedsession(session, {
  autoSave:true
}));

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

/*
app.use(express.bodyParser());
app.use(express.cookieParser('secret text'));
app.use(express.session({
    store: sessionStore,
    secret: 'secret',
    key: 'express.sid'}
));
*/

app.use(express.static(path.join(__dirname, 'public')));
app.use('/js', express.static(__dirname + '/node_modules/jquery/dist')); // redirect JS jQuery
app.use('/js', express.static(__dirname + '/node_modules/bootstrap/dist/js')); // redirect bootstrap JS
app.use('/public/js', express.static(__dirname + '/public/js')); // redirect bootstrap JS
app.use('/public/css', express.static(__dirname + '/public/css')); // redirect bootstrap JS
app.use('/css', express.static(__dirname + '/node_modules/bootstrap/dist/css')); // redirect CSS bootstrap
app.use('/css', express.static(__dirname + '/node_modules/jquery-ui/dist/css'));
app.use('/fonts', express.static(__dirname + '/node_modules/bootstrap/dist/fonts')); // redirect bootstrap JS



server.listen(port, "0.0.0.0", function() {
  console.log( 'it is listening at port %d', port );
  console.log(bdd_feathers.feathers.length + " stored feathers");
  // ajout des chocards aux data
  dataForTemplate.feathers = bdd_feathers.feathers;

});
// catch 404 and forward to error handler
// app.use(function(req, res, next) {
//   var err = new Error('Not Found');
//   err.status = 404;
//   next(err);
// });

var players = [];

/*----------------------------------------------------------------------
* A little socket layer
----------------------------------------------------------------------*/
io.on('connection', function (socket) {

	socket.data = {};

	socket.on("login", function(data) {
		// We get all player's data
		var player = getplayer(data.playerId);

		// We create player if he does not exists yet
		if (player == null) {
			console.log('#'+data.playerId+' a rejoint le jeu !');

			players.push({
				playerId: data.playerId
			});
			player = getplayer(data.playerId);
		}

		// We set the player id to the current socket
		socket.data.playerId = player.playerId;

		socket.emit('player', player);

		promptplayers();
  });

	/*---------------------------------------------------------------------- 
	 *  Some Socket example.
	 ----------------------------------------------------------------------*/
	socket.on('test', function(data) {
		// We get all player's data
		var player = getplayer(socket.data.playerId);

		if (player != null) {
			console.log('#'+player.playerId);

			// We do some super stuff, as updating a database

			// We tell everyone we have done something for someone !
			io.sockets.emit('somethingHappend', {
				// Some data changes 
				// id: data.id
			});
		}
	});


	/*---------------------------------------------------------------------- 
	 *  Socket Disconnection
	 ----------------------------------------------------------------------*/
  socket.on("disconnect", function() {
		if (socket.data) {
		console.log('Socket Logout of player ' + socket.data.playerId);
		}
  });

});

/*---------------------------------------------------------------------- 
 *  Some Function to deal with the player sessions
 ----------------------------------------------------------------------*/
function getplayer(playerId) {
	var r = null;
	players.forEach(function(player) {
		if (player.playerId == playerId) r = player;
	});
	return r;
}

function updateplayer(playerId, playerUpdated) {
	players.forEach(function(player, index) {
		if (player.playerId == playerId) {
			players[index] = playerUpdated;
		}
	});
}

function promptplayers() {
	console.log('-----------------------------------');
	console.log(''+players.length+' player(s):');
	players.forEach(function(player) {
		console.log(' > #'+player.playerId);
		console.log(JSON.stringify(player, null, 4));
	});
	console.log('-----------------------------------');
}

/*---------------------------------------------------------------------- 
 *  ROUTING - API
 ----------------------------------------------------------------------*/
app.get('/register', function(req, res) {
	// See if this mac_address is known in the database
	// If known => get the config back
	// If not, register as a default device.
 });

/*---------------------------------------------------------------------- 
 *  ROUTING - TEMPLATING & PAGES
 ----------------------------------------------------------------------*/
app.get('/', function(req, res) {
	res.render('index',{ data: dataForTemplate });
});


app.listen();
