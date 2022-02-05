class Translator
  class << self
    def coordinates_to_position(coords)
      # 0,0 -> a8
      # 7,0 -> a1

      col = "abcdefgh"[coords[1]]
      row = 8 - coords[0]

      "#{col}#{row}"
    end

    def position_to_coordinates(position)
      # a8 -> 0,0
      # a1 -> 7,0

      j = "abcdefgh".index(position[0])
      i = 8 - position[1].to_i

      [i, j]
    end
  end
end
