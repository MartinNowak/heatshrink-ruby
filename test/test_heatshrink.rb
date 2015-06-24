require 'minitest/autorun'
require 'heatshrink'
require 'securerandom'

class HeatshrinkTest < Minitest::Test
  def test_equal
    50.times do
      data = SecureRandom.random_bytes(rand(2000))
      [8, 9, 10, 11, 12].each do |ws|
        [3, 4, 5, 6, 7].each do |la|
          data2 = Heatshrink.compress(data, ws, la)
          assert_equal data, Heatshrink.uncompress(data2, ws, la)
        end
      end
    end
  end

  def test_raise_lookahead
    # lookahead bigger than window
    assert_raises(RuntimeError) { Heatshrink.compress('foobar', 8, 9) }
  end
end
