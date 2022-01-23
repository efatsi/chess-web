
require_relative 'piece.rb'

class Sliding < Piece

  def possible_moves(*directions)
    possible_moves = []
    dir_array = []
    directions.each do |dir|
      if dir == :diagonal # TODO refractor to Rook, Bishop, Queen classes
        dir_array += [[1,1],[1,-1], [-1, 1],[-1,-1]]
      elsif dir == :straight
        dir_array += [[1,0], [0,1], [-1, 0], [0,-1]]
      end
    end

    dir_array.each do |el|
      x, y = @pos
      movex, movey = el
      position = [movex + x, movey + y]


      while inbounds?(position) && valid_move?(position)
        possible_moves << position
        x, y = position
        break if kills?(position)
        position = [x + movex, y + movey]

      end
    end
    possible_moves
  end



end
