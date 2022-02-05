class CreateGames < ActiveRecord::Migration[7.0]
  def change
    create_table :games do |t|
      t.string :board_state, null: false
      t.string :current_player, default: "white", null: false
      t.string :moves, array: true, default: []

      t.timestamps
    end
  end
end
