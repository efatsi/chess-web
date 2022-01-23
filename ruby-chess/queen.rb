class Queen < Sliding

  def possible_moves

    super(:diagonal, :straight)

  end

  def to_s
    " ♛ "
  end

end
