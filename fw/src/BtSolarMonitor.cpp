#include "Particle.h"
#include <Bt/Sensors/INA219.h>
#include <Bt/Sensors/SensorArray.h>
#include <Bt/Core/Log.h>

SYSTEM_MODE(MANUAL);

SYSTEM_THREAD(ENABLED);


#define DELAY 1000

Serial1LogHandler logHandler(115200,LOG_LEVEL_ALL);

int sBlueLed = 7;
int sRedLed = 6;
bool sState = false;

#define NUMBER_OF_SENSORS 6

uint8_t sAddresses[NUMBER_OF_SENSORS] = {
         0x40,
         0x44,
         0x41,
         0x45,
         0x4C,
         0x48
};

std::array<Bt::Sensors::INA219, NUMBER_OF_SENSORS> sSensors{{
      {Wire, sAddresses[0], Bt::Sensors::INA219::Gain::GAIN_40MV, 32768,250},
      {Wire, sAddresses[1], Bt::Sensors::INA219::Gain::GAIN_40MV, 32768,250},
      {Wire, sAddresses[2], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
      {Wire, sAddresses[3], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
      {Wire, sAddresses[4], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
      {Wire, sAddresses[5], Bt::Sensors::INA219::Gain::GAIN_80MV,  8192, 50}
}};


Bt::Sensors::SensorArray<Bt::Sensors::INA219,Bt::Sensors::INA219Reading,NUMBER_OF_SENSORS> sSensorArray{sSensors,3};

uint32_t sLoopCounter = 0;

void toggle() {
   sState = !sState;
   digitalWrite(sBlueLed, sState ? HIGH : LOW);
   digitalWrite(sRedLed, sState ? HIGH : LOW);
}

void setup() {
  BT_CORE_LOG_INFO("*** Start ... ***");
  delay(2000);
  BT_CORE_LOG_INFO("*** ... GO    ***");
  Wire.setSpeed(CLOCK_SPEED_100KHZ);
  Wire.begin();
  pinMode(sBlueLed, OUTPUT);
  pinMode(sRedLed, OUTPUT);
  digitalWrite(sBlueLed, sState ? HIGH : LOW);
  digitalWrite(sRedLed, sState ? HIGH : LOW);
  for (Bt::Sensors::INA219& sensor : sSensors) {
        sensor.begin();
  }
}

void loop() {
   unsigned long timer = millis();
   sLoopCounter++;
   BT_CORE_LOG_INFO("loop %u [" __DATE__ " " __TIME__ "]", sLoopCounter );
   toggle();
   auto readings = sSensorArray.readAll();
   for (Bt::Sensors::INA219Reading& reading : readings) {
      BT_CORE_LOG_INFO("Reading - [%s] U = %f I = %f ", reading.valid ? "valid" : "invalid"  ,reading.busVoltage, reading.current);
   }
   timer = millis() - timer;
   BT_CORE_LOG_INFO("go to sleep after loop %u took %d ms", sLoopCounter, timer);
   System.sleep(A0,RISING,2);
}

