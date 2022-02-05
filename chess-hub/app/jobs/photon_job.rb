class PhotonJob
  include SuckerPunch::Job

  def perform(device_id, function_name, arg)
    puts "Starting photon call"
    sleep 3
    puts "actually now"
    photon(device_id).function(function_name, arg)
  rescue => e
    puts "Photon function call error: #{e.message}"
  ensure
    puts "finished"
  end

  private

  def photon(device_id)
    @photon ||= RubySpark::Device.new(device_id)
  end
end
