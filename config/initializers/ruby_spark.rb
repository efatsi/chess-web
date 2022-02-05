RubySpark.configuration do |config|
  config.access_token = Rails.application.credentials.particle_access_token
  config.timeout      = 5.seconds
end
