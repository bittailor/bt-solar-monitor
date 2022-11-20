//*************************************************************************************************
//
//  BITTAILOR.CH - PlatformMock
//
//*************************************************************************************************

#include "PlatformMock.h"

#include <stdarg.h>

#include "gmock/gmock.h"

//*************************************************************************************************

void pinMode(uint16_t pin, PinMode mode) {
}

PinMode getPinMode(uint16_t pin) {
   return PIN_MODE_NONE;
}
bool pinAvailable(uint16_t pin) {
   return false;
}
void digitalWrite(uint16_t pin, uint8_t value) {

}

int32_t digitalRead(uint16_t pin) {
   return 1;
}

int32_t pinReadFast(uint16_t _pin) {
   return 1;
}

bool attachInterrupt(uint16_t pin, wiring_interrupt_handler_t handler, InterruptMode mode, int8_t priority, uint8_t subpriority) {
   return true;
}

bool attachInterrupt(uint16_t pin, raw_interrupt_handler_t handler, InterruptMode mode, int8_t priority, uint8_t subpriority) {
   return true;
}

//*************************************************************************************************

Logger::Logger() {
   mFile = fopen ("PlatfromMockLog.txt","a");
   fprintf(mFile, "*** START Platfrom Mock **\n\n");
}

void Logger::trace(const char *fmt, ...) const {
   va_list args;
   va_start(args, fmt);
   fputs("T: ", mFile);
   vfprintf(mFile, fmt, args);
   fputc('\n', mFile);
   va_end(args);
}

void Logger::info(const char *fmt, ...) const {
   va_list args;
   va_start(args, fmt);
   fputs("I: ", mFile);
   vfprintf(mFile, fmt, args);
   fputc('\n', mFile);
   va_end(args);
}

void Logger::warn(const char *fmt, ...) const {
   va_list args;
   va_start(args, fmt);
   fputs("W: ", mFile);
   vfprintf(mFile, fmt, args);
   fputc('\n', mFile);
   va_end(args);
}

void Logger::error(const char *fmt, ...) const {
   va_list args;
   va_start(args, fmt);
   fputs("E: ", mFile);
   vfprintf(mFile, fmt, args);
   fputc('\n', mFile);
   va_end(args);
}

void Logger::log(const char *fmt, ...) const {
   va_list args;
   va_start(args, fmt);
   fputs("L: ", mFile);
   vfprintf(mFile, fmt, args);
   fputc('\n', mFile);
   va_end(args);
}

extern Logger Log;



Logger Log;

//*************************************************************************************************

const char* TIME_FORMAT_ISO8601_FULL = "%Y-%m-%dT%H:%M:%S%z";

std::string TimeClass::format(const char* format_spec) {
   return "2017-10-01T13:14:15Z";
}

time_t TimeClass::now() {
   time_t timer;
   time(&timer);
   return timer;
}

TimeClass Time;

//*************************************************************************************************

USARTSerial Serial1;

//*************************************************************************************************

void SystemClass::sleep(uint16_t wakeUpPin, InterruptMode edgeTriggerMode, long seconds) {

}

SystemSleepResult SystemClass::sleep(const SystemSleepConfiguration& config) {
   return SystemSleepResult{};
}

SystemClass System;

//*************************************************************************************************

uint32_t millis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint32_t micros() {
   return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//*************************************************************************************************

