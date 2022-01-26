class Board
  class InvalidMoveError < StandardError; end
  class InvalidFenError < StandardError; end
  INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

  attr_accessor :grid, :message

  def initialize(fen: INITIAL_FEN)
    @grid = Array.new(8) {Array.new(8)}
    @message = nil
    @over = false

    load_from_fen(fen)
  end

  def to_fen
    fen = ""

    8.times do |i|
      8.times do |j|
        fen += @grid[i][j].try(:to_fen) || "1"
      end
      fen += "/" unless i == 7
    end

    fen = fen.gsub("11111111", "8");
    fen = fen.gsub("1111111", "7");
    fen = fen.gsub("111111", "6");
    fen = fen.gsub("11111", "5");
    fen = fen.gsub("1111", "4");
    fen = fen.gsub("111", "3");
    fen = fen.gsub("11", "2");

    fen
  end

  def load_from_fen(fen)
    i = 0
    j = 0

    fen.split("").each do |char|
      if char.match(/[1-8]/)
        char.to_i.times do
          @grid[i][j] = nil
          j += 1
        end
      elsif char == "/"
        if j != 8
          raise InvalidFenError.new("Invalid FEN row: #{fen.split("/")[i]}")
        end

        i += 1
        j = 0
      else
        @grid[i][j] = Piece.from_fen(char, @grid, [i,j])
        j += 1
      end
    end

    if j != 8 || i != 7
      raise InvalidFenError.new("Incorrect number of something: row #{i}/7. col #{j}/8. fen #{fen}")
    end
  end

  def to_js_state
    positions = {}

    8.times do |i|
      8.times do |j|
        if piece = @grid[i][j]
          positions[piece.position] = piece.to_key
        end
      end
    end

    positions
  end

  def king_castling_moves(piece)
    castling_moves = []
    piece.castling.each do |castling_move|
      castling_moves << castling_move[0]
    end

    castling_moves
  end

  def attempt_move(instruction, color)
    start_pos, end_pos = instruction.split(/[-x]/)
    start_coords = Translator.position_to_coordinates(start_pos)
    end_coords   = Translator.position_to_coordinates(end_pos)

    move(start_coords, end_coords, color)
  end

  def move(start, end_pos, color)
    start_row, start_col = start
    end_row, end_col = end_pos
    moving_piece = @grid[start_row][start_col]

    if moving_piece.nil?
      raise InvalidMoveError.new("can't move something that isn't there.")
    end

    if moving_piece.color != color
      raise InvalidMoveError.new("it is #{color}'s turn, can't move #{moving_piece.color}'s piece.")
    end

    if check?(moving_piece.color)
      @message = "#{moving_piece.color} is in check!"
    elsif check?(moving_piece.opposite_color)
      @message = "#{moving_piece.opposite_color} is in check!"
    end

    possible_moves = moving_piece.possible_moves

    if moving_piece.is_a?(King) && !check?(moving_piece.color) && moving_piece.castling.length > 0
      possible_moves += king_castling_moves(moving_piece)
    end

    raise InvalidMoveError.new("that piece isn't allowed to move there") unless possible_moves.include?(end_pos)

    end_row, end_col = end_pos
    if moved_into_check?(moving_piece, end_pos)
      raise InvalidMoveError.new("Cannot move into check")
    end

    if moving_piece.is_a?(King) && king_castling_moves(moving_piece).include?(end_pos)
      moving_piece.castling.each do |castling_move|
        if castling_move[0] == end_pos
          rook_start = castling_move[1]
          rook_end = castling_move[2]
          move!(rook_start, rook_end)
          break
        end
      end
    end

    move!(start, end_pos)

    if check?(moving_piece.opposite_color)
      @message = "#{moving_piece.opposite_color} is in check!"
      if checkmate?(moving_piece.opposite_color)
        @message = "#{moving_piece.opposite_color} is in checkmate! Game over."
      end
    end

    if moving_piece.is_a?(Pawn) && moving_piece.promotion?
      @grid[end_row][end_col] = Queen.new(moving_piece.color, @grid, moving_piece.pos)
    elsif moving_piece.is_a?(Rook) || moving_piece.is_a?(King)
      moving_piece.castleable = false if moving_piece.castleable
    end
  end

  def moved_into_check?(moving_piece, end_pos)
    dup_board = self.dup
    dup_board.move!(moving_piece.pos, end_pos)
    dup_board.check?(moving_piece.color)
  end

  def dup
    dup_board = Board.new(fen: to_fen)
  end

  def move!(start_pos, end_pos)
    xstart, ystart = start_pos
    xend, yend = end_pos

    moved_piece = @grid[xstart][ystart]
    @grid[xend][yend] = moved_piece
    moved_piece.pos = end_pos

    @grid[xstart][ystart] = nil
  end

# pass in current_player color
  def check?(color)
    opposing_pieces = []
    king_piece = nil
    @grid.each do |row|
      row.each do |piece|
        if piece
          king_piece = piece if piece.is_a?(King) && piece.color == color
          opposing_pieces << piece if piece.color != color
        end
      end
    end
    opposing_pieces.each do |piece|
      return true if piece.possible_moves.include?(king_piece.pos)
    end
    @message = nil
    false
  end

  def checkmate?(color)
    return false unless check?(color)

    @grid.each do |row|
      row.each do |piece|
        if piece && piece.color == color
          piece.possible_moves.each do |move|
            return false if !moved_into_check?(piece, move)
          end
        end
      end
    end
    @over = true
    true
  end

  def in_bounds?(move)
    x, y = move
    x.between?(0, 7) && y.between?(0, 7)
  end

  def over?
    @over
  end
end
