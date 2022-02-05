class Bishop < Sliding

  def possible_moves
    dir = :diagonal
    super(dir)
  end

  def to_s
    " ♝ "
  end

  def piece_key
    "B"
  end
end
