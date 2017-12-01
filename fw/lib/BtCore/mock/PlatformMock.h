//*************************************************************************************************
//
//  BITTAILOR.CH - PlatformMock
//
//*************************************************************************************************

#ifndef INC__PlatformMock__h
#define INC__PlatformMock__h

#include <stdint.h>
#include <stddef.h>
#include <chrono>
#include "gmock/gmock.h"

//*************************************************************************************************

class Logger {
   public:
       Logger();
       void trace(const char *fmt, ...) const __attribute__((format(printf, 2, 3))); // First argument is implicit 'this'
       void info(const char *fmt, ...) const __attribute__((format(printf, 2, 3)));
       void warn(const char *fmt, ...) const __attribute__((format(printf, 2, 3)));
       void error(const char *fmt, ...) const __attribute__((format(printf, 2, 3)));
       void log(const char *fmt, ...) const __attribute__((format(printf, 2, 3)));
   private:
       FILE* mFile;
};
extern Logger Log;

//*************************************************************************************************

extern const char* TIME_FORMAT_ISO8601_FULL;

class TimeClass {
   public:
      std::string format(const char* format_spec);
};

extern TimeClass Time;  //eg. usage: Time.day();

//*************************************************************************************************

class Stream {
   public:
      virtual int available() = 0;
      virtual int read() = 0;

      virtual size_t println(const char[]) =0;
};

class StreamMock : public Stream {
 public:
  MOCK_METHOD0(available, int());
  MOCK_METHOD0(read, int());
  MOCK_METHOD1(println, size_t(const char[]));
};

//*************************************************************************************************

uint32_t millis();

//*************************************************************************************************



//class MillisMock {
// public:
//      MillisMock() {
//         ON_CALL(*this, millis()).WillByDefault(testing::Return(
//
//         ));
//      }
//      MOCK_METHOD0(millis, uint32_t());
//};
//
//extern MillisMock sMillisMock;









#endif // INC__PlatformMock__h
