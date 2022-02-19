class Game < ApplicationRecord
  has_many :photon_boards, dependent: :destroy

  def initialize(opts = {})
    super

    @board = Board.new
    self.state_history = [
      {
        fen: @board.to_fen,
        current_player: :white,
        message: @board.current_message(:white, :black)
      }
    ]
  end

  def self.most_recent
    self.last || self.create
  end

  def board
    @board ||= Board.new(fen: current_fen)
  end

  def process_move(instruction)
    begin
      board.attempt_move(instruction, current_player)

      # Update database
      self.update(state_history: state_history + [{
        move: instruction,
        fen: board.to_fen,
        current_player: next_player,
        message: board.current_message(next_player, current_player)
      }])

      # Update frontend (if connected)
      broadcast_game_state

      # Update other board (if connected)
      photon_boards.find_by(color: current_player).try(:alert, instruction)

      return { success: true, fen: current_fen, message: current_message }

    rescue Board::InvalidMoveError => e
      return { success: false, message: e.message, fen: current_fen }
    end
  end

  def reset_state(new_fen, wipe_history: false)
    if wipe_history
      self.update(state_history: [{
        fen: new_fen,
        current_player: current_player,
        message: board.current_message(current_player, next_player)
      }])
    else
      self.update(state_history: state_history + [{
        move: "RESET",
        fen: new_fen,
        current_player: current_player,
        message: board.current_message(current_player, next_player)
      }])

      reset_board
      broadcast_game_state
    end
  end

  def back(steps: 1, broadcast: true)
    last_index = -1 - steps
    self.update(state_history: self.state_history[0..last_index])

    reset_board
    broadcast_game_state if broadcast
  end

  def change_player
    self.state_history.last["message"] = board.current_message(next_player, current_player)
    self.state_history.last["current_player"] = next_player
    self.save
  end

  def current_player
    current_state[:current_player].to_sym
  end

  def next_player
    current_player == :white ? :black : :white
  end

  def current_fen
    current_state[:fen]
  end

  def current_message
    current_state[:message]
  end

  def current_state
    state_history.last.with_indifferent_access
  end

  private

  def broadcast_game_state
    ActionCable.server.broadcast("game_#{id}", current_state)
  end

  def reset_board
    # clear cached board which is now too far in the future
    @board = nil
  end
end
