require 'heatshrink/heatshrink'

module Heatshrink
  DEF_WINDOW_SZ2 = 11
  DEF_LOOKAHEAD_SZ2 = 4

  def self.compress data, window_sz2=DEF_WINDOW_SZ2, lookahead_sz2=DEF_LOOKAHEAD_SZ2
    Internal.compress data, window_sz2, lookahead_sz2
  end

  def self.uncompress data, window_sz2=DEF_WINDOW_SZ2, lookahead_sz2=DEF_LOOKAHEAD_SZ2
    Internal.uncompress data, window_sz2, lookahead_sz2
  end
end
