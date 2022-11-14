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
#include <functional>
#include "gmock/gmock.h"



//*************************************************************************************************


#define ATOMIC_BLOCK()

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

typedef enum InterruptMode {
  CHANGE,
  RISING,
  FALLING
} InterruptMode;


enum PinState {
    LOW = 0,
    HIGH = 1
};

typedef enum PinMode {
  INPUT,
  OUTPUT,
  INPUT_PULLUP,
  INPUT_PULLDOWN,
  AF_OUTPUT_PUSHPULL, //Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
  AF_OUTPUT_DRAIN,    //Used internally for Alternate Function Output Drain(I2C etc). External pullup resistors required.
  AN_INPUT,           //Used internally for ADC Input
  AN_OUTPUT,          //Used internally for DAC Output
  PIN_MODE_NONE=0xFF
} PinMode;

void pinMode(uint16_t pin, PinMode mode);
PinMode getPinMode(uint16_t pin);
bool pinAvailable(uint16_t pin);
void digitalWrite(uint16_t pin, uint8_t value);
int32_t digitalRead(uint16_t pin);

int32_t pinReadFast(uint16_t _pin);

typedef std::function<void()> wiring_interrupt_handler_t;
typedef void (*raw_interrupt_handler_t)(void);

bool attachInterrupt(uint16_t pin, wiring_interrupt_handler_t handler, InterruptMode mode, int8_t priority = -1, uint8_t subpriority = 0);
bool attachInterrupt(uint16_t pin, raw_interrupt_handler_t handler, InterruptMode mode, int8_t priority = -1, uint8_t subpriority = 0);
template <typename T>
bool attachInterrupt(uint16_t pin, void (T::*handler)(), T *instance, InterruptMode mode, int8_t priority = -1, uint8_t subpriority = 0) {
    using namespace std::placeholders;
    return attachInterrupt(pin, std::bind(handler, instance), mode, priority, subpriority);
}


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
