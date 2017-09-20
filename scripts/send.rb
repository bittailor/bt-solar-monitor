require 'socket'

host = 'localhost'
port = 8080

raw = [80, 90]
body = raw.pack('SS')
puts body.class.name
puts body.length
puts "body is '#{body}'"

s = TCPSocket.open host, port
s.puts "POST /binary/test HTTP/1.1\r\n"
s.puts 'Content-Type: application/octet-stream'
s.puts "Content-Length: #{body.length}"
s.puts body
s.puts "\r\n"

while line = s.gets
  puts line
end

s.close