class CreatePhotonBoards < ActiveRecord::Migration[7.0]
  def change
    create_table :photon_boards do |t|
      t.string :device_id, null: false
      t.string :color, null: false
      t.integer :game_id, null: false

      t.timestamps
    end
  end
end
