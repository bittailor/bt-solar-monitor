#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Stream {
public:
   Stream(char* pBuffer, size_t pCapacity)
   : mBuffer(pBuffer), mCapacity(pCapacity), mLenght(0){
      mBuffer[mLenght] = 0;
   }


   friend Stream& operator<<(Stream& pStream, int16_t pValue) {
      pStream.mBuffer[pStream.mLenght] = (pValue >> 8) & 0xff;
      pStream.mBuffer[pStream.mLenght+1] = pValue & 0xff;
      pStream.mLenght+=2;
      pStream.mBuffer[pStream.mLenght] = 0;
      return pStream;
   }


private:
   static int16_t scale(float value) {
     return static_cast<int16_t>(round(value * 1000));
  }

   char* mBuffer;
   size_t mCapacity;
   size_t mLenght;

};

int main(int argc, char* argv[])
{
  try
  {

    std::string host = "localhost";
    std::string port = "8080";
    
    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    char buffer[100] = {0};

    int16_t a = 16706;
    int16_t b = 17220;

    Stream bodystream(buffer,100);
    bodystream << a << b;

    std::cout << std::endl << "Buffer:" << strlen(buffer) << std::endl;
    std::cout.write(buffer,4);
    std::cout << std::endl << std::endl; 

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "POST /binary/test HTTP/1.0\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Content-Type: application/octet-stream\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "Content-Length: 4\r\n\r\n";
    request_stream.write(buffer,4);

    // Send the request.
    boost::asio::write(socket, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
      return 1;
    }
    if (status_code != 200)
    {
      std::cout << "Response returned with status code " << status_code << "\n";
      return 1;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
      std::cout << header << "\n";
    std::cout << "\n";

    // Write whatever content we already have to output.
    if (response.size() > 0)
      std::cout << &response;

    // Read until EOF, writing data to output as we go.
    boost::system::error_code error;
    while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
      std::cout << &response;
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }
  std::cout << "\n";
  return 0;
}