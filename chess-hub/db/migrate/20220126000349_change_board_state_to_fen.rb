class ChangeBoardStateToFen < ActiveRecord::Migration[7.0]
  def change
    remove_column :games, :board_state
    add_column :games, :fens, :string, array: true, default: []
  end
end
