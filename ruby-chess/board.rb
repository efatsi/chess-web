require_relative 'display.rb'
require_relative 'piece.rb'
require_relative 'pawn'
require_relative 'sliding'
require_relative 'stepping'
require_relative 'rook'
require_relative 'bishop'
require_relative 'knight'
require_relative 'king'
require_relative 'queen'

# require 'byebug'

class Board

  attr_accessor :grid, :message

  def initialize(populate = true)
    @grid = Array.new(8) {Array.new(8)}
    @message = nil
    @over = false
    populate_grid if populate
  end

  def populate_grid
    @grid.each_index do |i|
      if i == 0 || i == 7
        @grid[0][i] = Rook.new(:black, @grid, [0,i])
        @grid[7][i] = Rook.new(:white, @grid, [7,i])
      elsif i == 1 || i == 6
        @grid[0][i] = Knight.new(:black, @grid, [0,i])
        @grid[7][i] = Knight.new(:white, @grid, [7,i])
      elsif i == 2 || i == 5
        @grid[0][i] = Bishop.new(:black, @grid, [0,i])
        @grid[7][i] = Bishop.new(:white, @grid, [7,i])
      elsif i == 3
        @grid[0][i] = Queen.new(:black, @grid, [0,i])
        @grid[7][i] = Queen.new(:white, @grid, [7,i])
      else
        @grid[0][i] = King.new(:black, @grid, [0,i])
        @grid[7][i] = King.new(:white, @grid, [7,i])
      end
    end
    @grid[1].each_index do |i|
      @grid[1][i] = Pawn.new(:black, @grid, [1,i])
      @grid[6][i] = Pawn.new(:white, @grid, [6,i])
    end
  end

  def king_castling_moves(piece)
    castling_moves = []
    piece.castling.each do |castling_move|
      castling_moves << castling_move[0]
    end

    castling_moves
  end

  def move(start, end_pos, color)
    start_row, start_col = start
    moving_piece = @grid[start_row][start_col]

    if moving_piece.nil? || moving_piece.color != color
      raise ArgumentError
    end

    if check?(moving_piece.color)
      @message = "#{moving_piece.color} is in check!"
    elsif check?(moving_piece.opposite_color)
      @message = "#{moving_piece.opposite_color} is in check!"
    end

    possible_moves = moving_piece.possible_moves

    if moving_piece.is_a?(King) && !check?(moving_piece.color) && moving_piece.castling.length > 0
      possible_moves += king_castling_moves(moving_piece)
    end

    raise ArgumentError unless possible_moves.include?(end_pos)

    end_row, end_col = end_pos
    if moved_into_check?(moving_piece, end_pos)
      puts "Cannot move into check."
      sleep(1)
     raise ArgumentError.new
    end

    if moving_piece.is_a?(King) && king_castling_moves(moving_piece).include?(end_pos)
      moving_piece.castling.each do |castling_move|
        if castling_move[0] == end_pos
          rook_start = castling_move[1]
          rook_end = castling_move[2]
          move!(rook_start, rook_end)
          break
        end
      end
    end

    move!(start, end_pos)

    if check?(moving_piece.opposite_color)
      @message = "#{moving_piece.opposite_color} is in check!"
      if checkmate?(moving_piece.opposite_color)
        @message = "#{moving_piece.opposite_color} is in checkmate! Game over."
      end
    end

    if moving_piece.is_a?(Pawn) && moving_piece.promotion?
      @grid[row][col] = Queen.new(moving_piece.color, @grid, moving_piece.pos)
    elsif moving_piece.is_a?(Rook) || moving_piece.is_a?(King)
      moving_piece.castleable = false if moving_piece.castleable
    end

  end

  def moved_into_check?(moving_piece, end_pos)
    dup_board = self.dup
    dup_board.move!(moving_piece.pos, end_pos)
    dup_board.check?(moving_piece.color)
  end

  def dup
    dup_board = Board.new(false)
    @grid.each_index do |row|
      @grid[row].each_with_index do |piece, col|
        if piece.nil?
          dup_board.grid[row][col] = nil
        else
          dup_board.grid[row][col] = piece.dup(dup_board.grid)
        end
      end
    end
    dup_board
  end

  def move!(start_pos, end_pos)
    xstart, ystart = start_pos
    xend, yend = end_pos

    moved_piece = @grid[xstart][ystart]
    @grid[xend][yend] = moved_piece
    moved_piece.pos = end_pos

    @grid[xstart][ystart] = nil
  end

# pass in current_player color
  def check?(color)
    opposing_pieces = []
    king_piece = nil
    @grid.each do |row|
      row.each do |piece|
        if piece
          king_piece = piece if piece.is_a?(King) && piece.color == color
          opposing_pieces << piece if piece.color != color
        end
      end
    end
    opposing_pieces.each do |piece|
      return true if piece.possible_moves.include?(king_piece.pos)
    end
    @message = nil
    false
  end

  def checkmate?(color)
    return false unless check?(color)

    @grid.each do |row|
      row.each do |piece|
        if piece && piece.color == color
          piece.possible_moves.each do |move|
            return false if !moved_into_check?(piece, move)
          end
        end
      end
    end
    @over = true
    true
  end

  def in_bounds?(move)
    x, y = move
    x.between?(0, 7) && y.between?(0, 7)
  end

  def over?
    @over
  end


end
