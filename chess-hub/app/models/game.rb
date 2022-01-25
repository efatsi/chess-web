class Game < ApplicationRecord
  validates :board_state, presence: true

  def initialize(opts = {})
    super

    @board = Board.new(populate: true)
    self.board_state = board.to_state
  end

  def board
    @board ||= Board.new(populate: false, board_state: board_state)
  end

  def process_move(instruction)
    begin
      board.attempt_move(instruction, current_player)
      self.update({
        moves:          self.moves + [instruction],
        board_state:    board.to_state,
        current_player: current_player == :white ? :black : :white
      })

      return { success: true }

    rescue Board::InvalidMoveError => e
      return { success: false, message: e.message }
    end
  end

  def current_player
    super.to_sym
  end
end
