class Knight < Stepping

  def to_s
    " ♞ "
  end

  def possible_moves
    super(:knight)
  end

  def piece_key
    "N"
  end
end
