class King < Stepping

	attr_accessor :castleable

	def initialize(color, grid, pos)
		super(color, grid, pos)
		@castleable = true
	end

  def to_s
    " ♚ "
  end

  def possible_moves
    super(:king)
  end

  def castling
  	castling_moves = []

  	return castling_moves unless @castleable

  	x, y = @pos

  	# check that squares btwn king and rook1 are empty
  	if @grid[x][y-3] == nil && @grid[x][y-2] == nil && @grid[x][y-1] == nil
  		rook = @grid[x][y-4]
  		if rook.is_a?(Rook) && rook.color == color && rook.castleable
  			castling_moves << [[x, y-2], [x, y-4], [x, y-1]]
  		end
  	end

  	# check that squares btwn king and rook2 are empty
  	if @grid[x][y+1] == nil && @grid[x][y+2] == nil
  		rook = @grid[x][y+3]
  		if rook.is_a?(Rook) && rook.color == color && rook.castleable
  			castling_moves << [[x, y+2], [x, y+3], [x, y+1]]
  		end
  	end

  	return castling_moves
  end

end
