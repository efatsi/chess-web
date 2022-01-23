# ruby-chess

This is a 2-player implementation of chess, written entirely in Ruby.

To play, clone the repo and type "ruby game.rb" into your terminal.

Use arrow keys or WASD to move the cursor. Use enter or spacebar to select a piece. The selected piece will be highlighted in green. Use enter or spacebar to select the destination of the piece.

## Features
* All chesspieces inherit from a Piece class
  * Queen, Rook, and Bishop inherit from Sliding class
  * King, Knight, and Pawn inherit from Stepping class
* Error handling for improper moves
* Deep dup of chessboard and pieces to perform move validations without chainging game state

## Screenshots

#### Start
![start](http://res.cloudinary.com/catalyst/image/upload/v1457944442/gameStart_zo65oy.png)

#### Piece capture
![pre-capture](http://res.cloudinary.com/catalyst/image/upload/v1457944442/selectedPiece_ol2i3t.png)
![post-capture](http://res.cloudinary.com/catalyst/image/upload/v1457944442/eating_tr1fxk.png)

#### Castling
![pre-castling](http://res.cloudinary.com/catalyst/image/upload/v1457944442/pre-castling_tfbdho.png)

![post-castling](http://res.cloudinary.com/catalyst/image/upload/v1457944442/postcastling_zgbt5c.png)

### Check
![check](http://res.cloudinary.com/catalyst/image/upload/v1457944442/check_dbpvjb.png)

### Checkmate
![fools_mate](http://res.cloudinary.com/catalyst/image/upload/v1457944442/checkmate_vz1ebg.png)


## Future Features (TBD)
* En passant, draw
* Option to turn chessboard around for each player's turn
* Option to undo last move
* Show captured pieces
* Store famous chess games as sequence of moves; allow play-through
