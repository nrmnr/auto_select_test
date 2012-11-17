#! ruby
# coding: Windows-31j
require "rake/clean"

CC = "g++"
SRCS = FileList["**/*.cpp"]
OBJS = SRCS.ext("o")

CLEAN.include(OBJS)
CLOBBER.include("a.exe")

task :default => :test

task :test => "a.exe" do
	sh "a.exe"
end

file "a.exe" => OBJS do |t|
	sh "#{CC} -o #{t.name} #{t.prerequisites.join(' ')}"
end

rule ".o" => [".cpp", ".h"] do |t|
	sh "#{CC} -c #{t.source}"
end
