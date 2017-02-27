require 'em-websocket'
require 'json'
require 'serialport'

sp = begin
  SerialPort.new('/dev/tty.usbmodem1411', 9600, 8, 1, SerialPort::NONE)
rescue
  SerialPort.new('/dev/tty.usbmodem1421', 9600, 8, 1, SerialPort::NONE)
end

def message_from(sp)
  message = sp.gets
  message.chop!

  puts "Sending '#{message}'"
  { "event" => message }
end

EventMachine::WebSocket.start(:host => '0.0.0.0', :port => 8080) do |ws|
  ws.onclose { puts "WebSocket closed" }

  ws.onmessage do
    ws.send message_from(sp).to_json
  end
end
