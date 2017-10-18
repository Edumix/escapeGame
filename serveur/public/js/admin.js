function guidGenerator() {
  var S4 = function() {
     return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
  };
  return (S4()+"-"+S4()+"-"+S4());
}

/* * * * * * * * * */

var socket = io.connect('http://localhost:5000');

$('#button_refreshPlayersCookie').on('click', function(){
  socket.emit('callToRefreshPlayersCookie', {});
});

$('#button_refreshChocardsList').on('click', function(){
  socket.emit('refreshChocardsList', {});
});

$('#button_refreshPlayersCookie').on('click', function(){
  socket.emit('refreshPlayersCookie', {});
});
