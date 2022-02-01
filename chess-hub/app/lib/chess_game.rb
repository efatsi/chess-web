class ChessGame
  attr_reader :board

  def initialize
    @board = Board.new
    @display = Display.new(@board)
    @current_player = :white
  end

  def over?
    @board.over?
  end

  def play
    until over?
      begin
        pos_1 = @display.get_move(@current_player)
        pos_2 = @display.get_move(@current_player)
        @board.move(pos_1, pos_2, @current_player)
      rescue Board::InvalidMoveError => e
        puts e.message
        sleep(1)
        retry
      end

      @current_player = @current_player == :white ? :black : :white
    end
    puts @board.message
  end
end
