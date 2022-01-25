class GameChannel < ApplicationCable::Channel
  def subscribed
    @game = Game.find(params[:id])
    stream_from "game_#{@game.id}"
  end

  def receive(data)
    puts "WEBSOCKET RECEIVE: #{data}, #{@game.id}"
  end
end
