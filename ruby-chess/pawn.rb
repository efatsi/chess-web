require_relative 'piece.rb'

class Pawn < Piece

  def possible_moves

    move_arr = []
    row, col = @pos
    start_row = false

    if @color == :white
      adj = -1
      start_row = true if row == 6
    else
      adj = 1
      start_row = true if row == 1
    end
    move_arr << [row + adj, col] unless kills?([row + adj, col])
    move_arr << [row + (2 * adj), col] if start_row && !kills?([row + (2 * adj), col])

    move_arr += diagonals(row, col)
    move_arr.select { |move| inbounds?(move) && valid_move?(move) }
  end

  def diagonals(row, col)
    if @color == :black
      adjustor = 1
    else
      adjustor = -1
    end
    diagonals = [[adjustor, 1], [adjustor, -1]]
    diag_moves = []
    diagonals.each do |diag|
      diag_x, diag_y = diag
      if inbounds?([row + diag_x, col + diag_y]) && valid_move?([row + diag_x, col + diag_y])
        if @grid[row + diag_x][col + diag_y] && @grid[row + diag_x][col + diag_y].color != @color
          diag_moves << [row + diag_x, col + diag_y]
        end
      end
    end

    diag_moves
  end

  def promotion?
    return true if @color == :white && @pos[0] == 0
    return true if @color == :black && @pos[0] == 7
    false
  end


  def to_s
    " ♟ "
  end

end
