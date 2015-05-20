require 'rake/extensiontask'
require 'rake/testtask'

Rake::ExtensionTask.new 'heatshrink' do |ext|
  ext.lib_dir = 'lib/heatshrink'
end

Rake::TestTask.new do |t|
  t.libs << 'test'
end

desc "Run tests"
task :default => :test
