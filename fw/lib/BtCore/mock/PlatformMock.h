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

#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

// todo - this is corev1 specific, needs to go in a conditional define

#define A0 10
#define A1 11
#define A2 12
#define A3 13
#define A4 14
#define A5 15
#define A6 16

// WKP pin is also an ADC on Photon
#define A7 17

#define RX 18
#define TX 19

#define BTN 20

// WKP pin on Photon
#define WKP 17

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
      static time_t now();
      std::string format(const char* format_spec);
};

extern TimeClass Time;

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

typedef enum InterruptMode {
  CHANGE,
  RISING,
  FALLING
} InterruptMode;

//*************************************************************************************************

class SystemClass {
   public:
      static void sleep(uint16_t wakeUpPin, InterruptMode edgeTriggerMode, long seconds=0);
};

extern SystemClass System;

//*************************************************************************************************

uint32_t millis();
uint32_t micros();

//*************************************************************************************************

#define __WFI()

//*************************************************************************************************


typedef std::string String;

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
