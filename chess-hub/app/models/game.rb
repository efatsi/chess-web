class Game < ApplicationRecord
  INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

  validate :fens_and_moves

  def initialize(opts = {})
    super

    @board = Board.new
    self.fens = [@board.to_fen]
  end

  def board
    @board ||= Board.new(fen: fens.last)
  end

  def process_move(instruction)
    begin
      board.attempt_move(instruction, current_player)
      self.update({
        moves:          self.moves + [instruction],
        fens:           self.fens + [board.to_fen],
        current_player: current_player == :white ? :black : :white
      })
      ActionCable.server.broadcast("game_#{id}", { move: instruction.gsub("x", "-") })

      return { success: true }

    rescue Board::InvalidMoveError => e
      return { success: false, message: e.message }
    end
  end

  def current_player
    super.to_sym
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
end
