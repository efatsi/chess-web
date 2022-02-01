class PhotonBoard < ApplicationRecord
  belongs_to :game

  validates :color, inclusion: {
    in: ["white", "black"],
    message: "'%{value}' is invalid. Must be 'white' or 'black'."
  }

  def alert(instruction)
    photon.function("other-move", instruction)
  rescue => e
    # Timeout isn't working
    # should probably move to background job anyway
    puts "Photon function call error: #{e.message}"
  end

  private

  def photon
    @photon ||= RubySpark::Device.new(device_id)
  end
end
