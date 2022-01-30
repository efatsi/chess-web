class ChangeGameStateToJson < ActiveRecord::Migration[7.0]
  def change
    remove_column :games, :moves, :string, array: true, default: []
    remove_column :games, :fens, :string, array: true, default: []
    remove_column :games, :current_player, :string, default: "white"

    add_column :games, :state_history, :json, array: true, default: []
  end
end
