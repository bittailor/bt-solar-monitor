#include <functional>
#include <Particle.h>
#include <cellular_hal.h>
#include <nokia-5110-lcd.h>
#include <Bt/Core/Log.h>
#include <Bt/Sensors/INA219.h>
#include <Bt/Sensors/SensorArray.h>
#include <Bt/SolarMonitor/AveragingFilter.h>
#include <Bt/SolarMonitor/StorageFilter.h>
#include <Bt/SolarMonitor/MessageFilter.h>

#define MEASURE_SLEEP 2
#define AVERAGE_MINUTES 5
#define STOARGE_HOURS 1

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

SYSTEM_MODE(MANUAL);
STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));

//STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
//SYSTEM_THREAD(ENABLED);

Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);

//===
//retained uint32_t sLoopCounter = 0;
//retained uint8_t sSensorState = 0;
//===

uint32_t sLoopCounter = 0;

int sBlueLed = 7;

uint8_t sAddresses[] = {
         0x40,
         0x44,
         0x41,
         0x45,
         0x4C,
         0x48
};

static const size_t NUMBER_OF_SENSORS = sizeof(sAddresses)/sizeof(sAddresses[0]);

std::array<Bt::Sensors::INA219, NUMBER_OF_SENSORS> sSensors{{
   {Wire, sAddresses[0], Bt::Sensors::INA219::Gain::GAIN_40MV, 32768,250},
   {Wire, sAddresses[1], Bt::Sensors::INA219::Gain::GAIN_40MV, 32768,250},
   {Wire, sAddresses[2], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
   {Wire, sAddresses[3], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
   {Wire, sAddresses[4], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
   {Wire, sAddresses[5], Bt::Sensors::INA219::Gain::GAIN_80MV,  8192, 50}
}};

Bt::Sensors::SensorArray<Bt::Sensors::INA219,Bt::Sensors::INA219Reading,NUMBER_OF_SENSORS> sSensorArray{sSensors,3};


static const size_t STORAGE_SIZE = ((STOARGE_HOURS*60)/AVERAGE_MINUTES);


typedef Bt::SolarMonitor::StorageFilter<STORAGE_SIZE> StorageFilter;
typedef Bt::SolarMonitor::MessageFilter<STORAGE_SIZE> MessageFilter;


MessageFilter sMessageFilter;
StorageFilter sStorageFilter(std::bind(&MessageFilter::consume,&sMessageFilter, std::placeholders::_1));
Bt::SolarMonitor::AveragingFilter sAveragingFilter(((AVERAGE_MINUTES*60)/MEASURE_SLEEP),
                                                   std::bind(&StorageFilter::consume,&sStorageFilter, std::placeholders::_1));


// CE
// RST
// DC
// BackLight

void setCharging(bool enable);
void tryPublish();

Nokia5110LCD::Display sDisplay(A2, D6, D5, A0);

FuelGauge sFuelGauge;

void setup() {
   BT_CORE_LOG_INFO("*** bt-solar-monitor***");
   setCharging(false);
   Wire.setSpeed(CLOCK_SPEED_100KHZ);
   Wire.begin();
   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);
   sDisplay.begin();
   sDisplay.setContrast(55); // Pretty good value, play around with it
   sDisplay.updateDisplay(); // with displayMap untouched, SFE logo

   BT_CORE_LOG_INFO("!!! Cellular.on()  !!!");
   Cellular.on();
   BT_CORE_LOG_INFO("!!! Cellular.off()  !!!");
   Cellular.off();
   BT_CORE_LOG_INFO("!!! Cellular DONE  !!!");

   BT_CORE_LOG_INFO("!!! FuelGauge.sleep()  !!!");
   sFuelGauge.sleep();
   BT_CORE_LOG_INFO("!!! FuelGauge DONE  !!!");

   for (Bt::Sensors::INA219& sensor : sSensors) {
      sensor.begin();
   }
}

void loop() {
   digitalWrite(sBlueLed, HIGH);
   unsigned long timer = millis();
   sLoopCounter++;
   //BT_CORE_LOG_INFO("loop a %u [" __DATE__ " " __TIME__ "]", sLoopCounter );

   auto readings = sSensorArray.readAll();
   sDisplay.clearDisplay();
   sDisplay.setStr(String::format("Loop %d",sLoopCounter), 0, 0, BLACK);
   for(std::size_t i = 0 ; i < readings.size() ; i++) {
      Bt::Sensors::INA219Reading& reading = readings[i];
      //BT_CORE_LOG_INFO("Reading - [%s] U = %f I = %f ", reading.valid ? "valid" : "invalid"  ,reading.busVoltage, reading.current);
      sDisplay.setStr(String::format("%.2f %.3f",reading.busVoltage,reading.current), 0, (i+1)*8, BLACK);
      sAveragingFilter.consume(Bt::SolarMonitor::MeasurementRecord{readings});
   }
   sDisplay.updateDisplay();
   timer = millis() - timer;
   //BT_CORE_LOG_INFO("go to sleep after loop %u took %d ms", sLoopCounter, timer);
   //BT_CORE_LOG_WARN("%d ms",timer);

   if((sLoopCounter % 1800) == 1) {
      tryPublish();
   }

   Serial1.flush();
   digitalWrite(sBlueLed, LOW);
   System.sleep(A0, RISING, MEASURE_SLEEP);
   //System.sleep(SLEEP_MODE_SOFTPOWEROFF,2);
}

void setCharging(bool enable) {

   PMIC pmic;

   // DisableCharging turns of charging. DisableBATFET completely disconnects the battery.
   if (enable) {
      pmic.enableCharging();
      pmic.enableBATFET();
   }
   else {
      pmic.disableCharging();
      pmic.disableBATFET();
   }

   // Disabling the watchdog is necessary, otherwise it will kick in and turn
   // charing at random times, even when sleeping.

   // This disables both the watchdog and the charge safety timer in
   // Charge Termination/Timer Control Register REG05
   // pmic.disableWatchdog() disables the watchdog, but doesn't disable the
   // charge safety timer, so the red LED will start blinking slowly after
   // 1 hour if you don't do both.
   byte DATA = pmic.readChargeTermRegister();

   if (enable) {
      DATA |= 0b00111000;
   }
   else {
      // 0b11001110 = disable watchdog
      // 0b11000110 = disable watchdog and charge safety timer
      DATA &= 0b11000110;
   }

   // This would be easier if pmic.writeRegister wasn't private (or disable
   // charge safety timer had an exposed method
   Wire3.beginTransmission(PMIC_ADDRESS);
   Wire3.write(CHARGE_TIMER_CONTROL_REGISTER);
   Wire3.write(DATA);
   Wire3.endTransmission(true);

}

void tryPublish() {
   unsigned long timer = millis();
   BT_CORE_LOG_INFO("Cellular.on() ...");
   Cellular.on();
   BT_CORE_LOG_INFO("... Cellular.on() DONE");
   BT_CORE_LOG_INFO("Cellular.connect() ...");
   Cellular.connect();
   BT_CORE_LOG_INFO("... Cellular.connect() DONE");
   while(! Cellular.ready() &&  Cellular.connecting()) {
      BT_CORE_LOG_INFO("Check - Cellular.ready()");
      Particle.process();
   }
   Particle.process();
   CellularSignal sig = Cellular.RSSI();
   BT_CORE_LOG_INFO("RSSI %d -dBm QUAL %d dB ", sig.rssi, sig.qual);

   BT_CORE_LOG_INFO("Cellular.connect() ...");
   Particle.connect();
   BT_CORE_LOG_INFO("... Cellular.connect() DONE");

   BT_CORE_LOG_INFO("while(!Particle.connected() ...");
   while(!Particle.connected()) {
      Particle.process();
   }
   BT_CORE_LOG_INFO("... while(!Particle.connected() DONE");

   for (int msgCounter = 0; msgCounter < 8; ++msgCounter) {

      const size_t MESSAGE_SIZE = 250;
      char message[MESSAGE_SIZE+1] = {0};

      size_t used = snprintf(message, MESSAGE_SIZE, "loop %lu.%d : ", sLoopCounter, msgCounter);
      for (size_t i = used; i < MESSAGE_SIZE ; ++i) {
         message[i] = ((i-used) % 56) + 65;
      }
      message[MESSAGE_SIZE] = 0;

      BT_CORE_LOG_INFO("Particle.publish(%d) %d ...", msgCounter, strlen(message));
      bool ack = Particle.publish("e/2/loop", message, WITH_ACK);
      BT_CORE_LOG_INFO(" ... Particle.publish(%d) %d", msgCounter, ack);
      Particle.process();

   }

   BT_CORE_LOG_INFO("Cellular.disconnect() ...");
   Particle.disconnect();
   BT_CORE_LOG_INFO("... Cellular.disconnect() DONE");

   BT_CORE_LOG_INFO("while(Particle.connected() ...");
   while(Particle.connected()) {
      Particle.process();
   }
   BT_CORE_LOG_INFO("... while(Particle.connected() DONE");

   BT_CORE_LOG_INFO("Cellular.disconnect() ...");
   Cellular.disconnect();
   BT_CORE_LOG_INFO("... Cellular.disconnect() DONE");
   BT_CORE_LOG_INFO("Cellular.off() ...");
   Cellular.off();
   BT_CORE_LOG_INFO("... Cellular.off() DONE");
   timer = millis() - timer;

   BT_CORE_LOG_INFO("publish took %lu ms",timer);
   BT_CORE_LOG_INFO("free memory: %lu", System.freeMemory());
}


