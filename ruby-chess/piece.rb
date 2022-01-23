class Piece

  attr_accessor :pos
  attr_reader :color, :grid

  def initialize(color, grid, pos)
    @color = color
    @grid = grid
    @pos = pos
  end

  def opposite_color
    @color == :black ? :white : :black
  end

  def valid_move?(move)
    x, y = move
    return false if @grid[x][y] && @grid[x][y].color == @color
    true
  end

  def kills?(position)
    x, y = position

    return true if @grid[x][y] && @grid[x][y].color != @color

    false
  end

  def inbounds?(move)
    x, y = move
    return true if x.between?(0, 7) && y.between?(0, 7)
    false
  end

  def dup(grid)
    piece_class = self.class

    if piece_class == Pawn
      duped = Pawn.new(@color, grid, @pos)
    elsif piece_class == Rook
      duped = Rook.new(@color, grid, @pos)
    elsif piece_class == Bishop
      duped = Bishop.new(@color, grid, @pos)
    elsif piece_class == Knight
      duped = Knight.new(@color, grid, @pos)
    elsif piece_class == Queen
      duped = Queen.new(@color, grid, @pos)
    elsif piece_class == King
      duped = King.new(@color, grid, @pos)
    end
    duped
  end

end
