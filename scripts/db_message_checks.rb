require 'json'
require 'mysql'


limit = 2000;

secrets = JSON.parse(File.read("../secrets.json"))
connection = Mysql.new secrets['solar']['db']['remoteHost'], secrets['solar']['db']['user'], secrets['solar']['db']['pw'], 'bockli_solar_db'

result = connection.query("SELECT * FROM RawMessages ORDER BY ts DESC LIMIT #{limit}")

messages = []

result.each do |row|
    messages.push(JSON.parse(row[2]))
end

ids = messages.collect{|m| m['data'].split('|').first.to_i}
size = ids.size
ids.uniq!.sort!

puts "messages #{size}"
puts "max #{ids.last}"
puts "min #{ids.first}"
puts "duplicates #{size - ids.size}"



(ids.size-1).downto(1).each do |i|
    if ids[i] != ids[i-1] + 1
        puts "!! #{i-1}=#{ids[i-1]} .. #{i}=#{ids[i]} ==> + #{ids[i] - ids[i-1]} !"
    end
end
