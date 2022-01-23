require_relative 'piece.rb'

class Stepping < Piece

  def possible_moves(type)
    moves_arr = []
    row, col = @pos
    if type == :king
      positions = [[-1,-1], [-1,0], [-1,1], [0,1], [0,-1], [1,-1], [1,0], [1,1]]
    else
      positions = [[1,2], [1,-2], [-1,2], [-1,-2], [2,1], [2,-1], [-2,1], [-2,-1]]
    end
    positions.each do |pos_arr|
      x, y = pos_arr
      new_move = [row + x, col + y]
      if inbounds?(new_move) && valid_move?(new_move)
        moves_arr << new_move
      end
    end
    moves_arr
  end

end
