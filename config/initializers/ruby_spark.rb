RubySpark.configuration do |config|
  config.access_token = ENV["PARTICLE_TOKEN"] || Rails.application.credentials.particle_access_token
  config.timeout      = 5.seconds
end
