class Game < ApplicationRecord
  INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

  validate :fens_and_moves

  def initialize(opts = {})
    super

    @board = Board.new
    self.fens = [@board.to_fen]
  end

  def board
    @board ||= Board.new(fen: current_fen)
  end

  def process_move(instruction)
    begin
      board.attempt_move(instruction, current_player)
      self.update({
        moves:          self.moves + [instruction],
        fens:           self.fens + [board.to_fen],
        current_player: next_player
      })
      broadcast_game_state

      return { success: true }

    rescue Board::InvalidMoveError => e
      return { success: false, message: e.message }
    end
  end

  def reset_state(new_fen, wipe_history: false)
    if wipe_history
      self.update(
        fens: [new_fen],
        moves: []
      )
    else
      self.update(
        fens: self.fens + [new_fen],
        moves: self.moves + ["RESET"]
      )
      broadcast_game_state
    end
  end

  def current_player
    super.to_sym
  end

  def next_player
    current_player == :white ? :black : :white
  end

  def last_move
    moves.last
  end

  def current_fen
    fens.last
  end

  def back(steps = 1)
    last_index = -1 - steps
    self.update(
      moves: self.moves[0..last_index],
      fens: self.fens[0..last_index],
      current_player: steps.odd? ? next_player : current_player
    )

    # clear cached board which is now too far in the future
    @board = nil
    broadcast_game_state
  end

  def current_message
    board.current_message(current_player, next_player)
  end

  private

  def fens_and_moves
    if fens.length < 1
      errors.add(:fens, "must have at least one fen")
    end

    if moves.length != fens.length - 1
      errors.add(:moves, "length should be #{fens.length - 1} (1 less then length of fens)")
    end
  end

  def broadcast_game_state
    ActionCable.server.broadcast("game_#{id}", {
      move: last_move,
      fen: current_fen,
      message: current_message
    })
  end
end
