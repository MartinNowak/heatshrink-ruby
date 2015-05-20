Gem::Specification.new do |s|
  s.name        = 'heatshrink'
  s.version     = '0.1.0'
  s.summary     = 'data compression library for embedded/real-time systems'
  s.authors     = ['Scott Vokes', 'Martin Nowak']
  s.email       = 'code@dawg.eu'
  s.homepage    = 'http://github.com/MartinNowak/heatshrink-ruby'
  s.files = Dir.glob("ext/**/*.{h,c,rb}") +
    Dir.glob("lib/**/*.rb")
  s.extensions << 'ext/heatshrink/extconf.rb'
  s.add_development_dependency 'rake-compiler', '~>0'
  s.license     = 'ICS'
end
