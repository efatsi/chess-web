$(document).ready(function() {
  var opts = {
    position: {
      c7: 'wB',
      d7: 'wK',
      e7: 'wN',
      f7: 'wR',
      g7: 'wP',
      a3: 'bB',
      c3: 'bP',
      d3: 'bQ',
      e3: 'bP',
      f3: 'bP',
      g3: 'bN',
    },
    draggable: true
  };
  var board = ChessBoard('board', opts);

  ws = new WebSocket("ws://localhost:8080");

  ws.onopen = function() {
    ws.send("hello server");
  };

  ws.onmessage = function(event) {
    console.log(event.data)
    var message = JSON.parse(event.data);
    console.log(message.event)

    board.move(message.event)

    // send a dummy message back to initiate
    // the onmessage callback again
    ws.send("next message please!");
  }
});
