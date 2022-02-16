class PhotonBoard < ApplicationRecord
  belongs_to :game

  validates :color, inclusion: {
    in: ["white", "black"],
    message: "'%{value}' is invalid. Must be 'white' or 'black'."
  }

  def alert(move, fen)
    data = {
      move: move.gsub("-", " - "),
      fen: fen
    }.to_json

    PhotonJob.perform_async(device_id, "other-move", data)
  end
end
