task :default => :gen

desc "gen files"
task :gen do
    sh 'sh scripts/gen.sh'
end

desc "build files"
task :build do
    sh 'mkdir build && cd build && cmake .. && make'
end

desc "mv main"
task :main => [:gen, :build] do
    sh "mv build/rocksDbDemo main"
end

