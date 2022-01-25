class GamesController < ApplicationController
  skip_before_action :verify_authenticity_token

  def index
    @games = Game.all
  end

  def show
    @game = Game.find(params[:id])
  end

  def move
    game = Game.find(params[:game_id])

    # example move: a2-a8
    move = game.process_move(params[:move])
    render json: move

    # move = Move.new(game, params[:move])
    # render json: {
    #   success: move.success,
    #   message: move.message
    # }
  end
end
