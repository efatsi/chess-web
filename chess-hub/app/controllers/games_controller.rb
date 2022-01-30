class GamesController < ApplicationController
  skip_before_action :verify_authenticity_token

  def new
    @game = Game.create
    redirect_to game_path(@game)
  end

  def index
    @games = Game.all
  end

  def show
    @game = Game.find(params[:id])
  end

  def move
    game = Game.find(params[:game_id])

    # example moves: a2-a8, d3xd4
    move = game.process_move(params[:move])
    render json: move

    # move = Move.new(game, params[:move])
    # render json: {
    #   success: move.success,
    #   message: move.message
    # }
  end

  def update
    game = Game.find(params[:game_id])
    new_fen = params[:fen]

    response = if params[:force] == "true"
      game.reset_state(new_fen, wipe_history: params[:wipe])
      { success: true }
    elsif move = game.board.valid_move_to_get_to(new_fen)
      game.process_move(move)
    else
      { success: false, message: "cannot find a possible move for that state" }
    end

    render json: response
  end

  def back
    game = Game.find(params[:game_id])

    game.back(broadcast: false)
    render json: { success: true }
  end

  def change_player
    game = Game.find(params[:game_id])
    game.change_player

    render json: { success: true, message: game.current_message }
  end
end
