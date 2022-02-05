class Rook < Sliding

  attr_accessor :castleable

  def initialize(color, grid, pos)
    super(color, grid, pos)
    @castleable = true
  end

  def possible_moves
    dir = :straight
    super(dir)
  end

  def to_s
    " ♜ "
  end

  def piece_key
    "R"
  end

end
