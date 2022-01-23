class Game < ApplicationRecord
  validates :board_state, presence: true
end
