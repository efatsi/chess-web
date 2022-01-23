class Piece

  attr_accessor :pos
  attr_reader :color, :grid

  def initialize(color, grid, pos)
    @color = color
    @grid = grid
    @pos = pos
  end

  def self.from_key(key, grid, pos)
    return if key == "--"

    color = case key[0]
    when "w"
      :white
    when "b"
      :black
    else
      raise "Unreadable color from key: #{key}"
    end

    klass = case key[1]
    when "K"
      King
    when "Q"
      Queen
    when "R"
      Rook
    when "N"
      Knight
    when "B"
      Bishop
    when "P"
      Pawn
    else
      raise "Unreadable klass from key: #{key}"
    end

    klass.new(color, grid, pos)
  end

  def to_state
    "#{@color.to_s.first}#{piece_key}"
  end

  def inspect
    to_state
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
end
