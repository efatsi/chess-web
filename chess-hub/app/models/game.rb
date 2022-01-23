class Game < ApplicationRecord
  validates :board_state, presence: true

  def initialize(opts = {})
    super

    @board = Board.new
    self.board_state = board.to_state
  end

  def board
    @board ||= Board.new(populate: false, board_state: board_state)
  end
end
