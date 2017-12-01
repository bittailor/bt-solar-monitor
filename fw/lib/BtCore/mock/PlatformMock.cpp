//*************************************************************************************************
//
//  BITTAILOR.CH - PlatformMock
//
//*************************************************************************************************

#include "PlatformMock.h"
#include "gmock/gmock.h"

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

TimeClass Time;

//*************************************************************************************************

uint32_t millis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//*************************************************************************************************

