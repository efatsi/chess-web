$(document).ready(function() {
  var opts = {
    position: {
      a1: 'wQ',
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
