class Piece

  attr_accessor :pos
  attr_reader :color, :grid

  def initialize(color, grid, pos)
    @color = color
    @grid = grid
    @pos = pos
  end

  def to_fen
    if color == :white
      piece_key.upcase
    else
      piece_key.downcase
    end
  end

  def self.from_fen(fen, grid, pos)
    color = if fen == fen.upcase
      :white
    else
      :black
    end

    klass = case fen.upcase
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
      raise "Unreadable klass from fen: #{fen}"
    end

    klass.new(color, grid, pos)
  end

  def to_key
    "#{@color.to_s.first}#{piece_key}"
  end

  def inspect
    to_key
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
    return false unless inbounds?(position)

    x, y = position

    return true if @grid[x][y] && @grid[x][y].color != @color

    false
  end

  def inbounds?(move)
    x, y = move
    return true if x.between?(0, 7) && y.between?(0, 7)
    false
  end

  def position
    Translator.coordinates_to_position(pos)
  end
end
