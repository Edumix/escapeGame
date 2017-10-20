function guidGenerator() {
  var S4 = function() {
     return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
  };
  return (S4()+"-"+S4()+"-"+S4());
}

/* * * * * * * * * */

var socket = io.connect('http://localhost:5000');

logPlayer();

function logPlayer(forceNewCookie = false) {
  if (Cookies.get('player') == undefined || forceNewCookie == true) {
    Cookies.set('player',guidGenerator());
  }

  console.log('Player: ' + Cookies.get('player'));

  var playerId = Cookies.get('player');

  socket.emit('login', {
    playerId: playerId
  });
}

socket.on('refreshPlayersCookie', function(data) {
  logPlayer(true);
});
