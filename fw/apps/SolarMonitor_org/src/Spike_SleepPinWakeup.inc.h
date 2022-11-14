#include <functional>

#include <Particle.h>
#include <Serial4/Serial4.h>
#include <Serial5/Serial5.h>

#include <Bt/Sensors/VeDirect/Registers.h>

using namespace Bt::Sensors::VeDirect;

SYSTEM_MODE(MANUAL);
Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);

uint32_t sLoopCounter = 0;
int sBlueLed = 7;

void setup() {
   Log.info("*** Spike SleepPinWakeup ***");
   pinMode(sBlueLed, OUTPUT);
   pinMode(A0, INPUT_PULLDOWN);
}


void loop() {
   int32_t pin = pinReadFast(A0);
   digitalWrite(sBlueLed, HIGH);
   Log.info("loop enter pin = %ld  time = %s", pin, Time.timeStr().c_str());

   delay(1000);

   Log.info("=>sleep %s", Time.timeStr().c_str());
   Serial1.flush();
   digitalWrite(sBlueLed, LOW);
   System.sleep(A0, RISING, 20);
}

