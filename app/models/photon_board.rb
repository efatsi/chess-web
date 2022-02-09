class PhotonBoard < ApplicationRecord
  belongs_to :game

  validates :color, inclusion: {
    in: ["white", "black"],
    message: "'%{value}' is invalid. Must be 'white' or 'black'."
  }

  def alert(instruction)
    PhotonJob.perform_async(device_id, "other-move", instruction.gsub("-", " - "))
  end
end
