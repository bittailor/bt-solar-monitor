#include "Particle.h"
#include <Bt/Sensors/INA219.h>

SYSTEM_THREAD(ENABLED);

#define DELAY 1000

int sBlueLed = 7;
int sRedLed = 6;
bool sState = false;

Bt::Sensors::INA219 sCurrentSensor;

void setup() {
  pinMode(sBlueLed, OUTPUT);
  pinMode(sRedLed, OUTPUT);
  digitalWrite(sBlueLed, sState ? HIGH : LOW);
  digitalWrite(sRedLed, sState ? HIGH : LOW);
}

void toggle() {
   sState = !sState;
   digitalWrite(sBlueLed, sState ? HIGH : LOW);
   digitalWrite(sRedLed, sState ? HIGH : LOW);
}

void loop() { 
   toggle();
   delay(1000);
}

