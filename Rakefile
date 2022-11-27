require 'fileutils'

host_test_dir = "fw/target/HostTest"
directory host_test_dir
namespace :HostTest do 
    task :build => [host_test_dir] do
        Dir.chdir(host_test_dir) do
            sh "cmake -DCMAKE_BUILD_TYPE=Debug ../.."
            sh "make check"    
        end
    end
end 
task :build => "HostTest:build"

Dir.glob('fw/apps/**/project.properties').each do|project_properties|
    # next unless File.directory?(f)
    project_folder = File.dirname(project_properties)
    project_name = File.basename(project_folder)
    namespace project_name do  
        task :build do
            puts ">> build #{project_name}"
            Dir.chdir(project_folder) do
                sh "neopo build"
            end
        end
        task :flash do
            puts ">> flash #{project_name}"
            Dir.chdir(project_folder) do
                sh "neopo flash"
            end
        end
    end
    task :build => "#{project_name}:build"
end



task :default => :build
task :flash => "SolarMonitor:flash"