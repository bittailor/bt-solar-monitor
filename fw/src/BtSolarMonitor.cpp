#include <functional>
#include <Particle.h>

#if PLATFORM_ID == 10
#include <cellular_hal.h>
#endif

#include <nokia-5110-lcd.h>
#include <Bt/Core/Log.h>
#include <Bt/Core/Cloud.h>
#include <Bt/Sensors/INA219.h>
#include <Bt/Sensors/SensorArray.h>
#include <Bt/SolarMonitor/AveragingFilter.h>
#include <Bt/SolarMonitor/StorageFilter.h>
#include <Bt/SolarMonitor/MessageFilter.h>
#include <Bt/SolarMonitor/ForkFilter.h>
#include <Bt/SolarMonitor/DisplayFilter.h>
#include <Bt/SolarMonitor/ValidateFilter.h>
#include <Bt/SolarMonitor/PublishFilter.h>

// ==== <Configuration> ==========

#define MEASURE_SLEEP 1

const size_t AVERAGE_SECONDS = 5  * 10;  // 5 *  60;
const size_t STOARGE_SECONDS = 10 * 60;  // 60 * 60;

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

#define EVENT_NAME_DATA "solar/data"
#define EVENT_NAME_STATUS "solar/status"

// ==== </Configuration> ==========

SYSTEM_MODE(MANUAL);

#if PLATFORM_ID == 10
STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));
#endif

#if PLATFORM_ID == 10
   #define Radio Cellular
#else
   #define Radio WiFi
#endif





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

Bt::Sensors::SensorArray<Bt::Sensors::INA219,Bt::Sensors::INA219Reading,NUMBER_OF_SENSORS> sSensorArray{sSensors,1};

// CE
// RST
// DC
// BackLight
Nokia5110LCD::Display sDisplay(A2, D6, D5, A0);

static const size_t STORAGE_SIZE = (STOARGE_SECONDS/AVERAGE_SECONDS);

typedef Bt::Core::Cloud<decltype(Radio),decltype(Particle)> Cloud;
typedef Bt::SolarMonitor::PublishFilter<Cloud> PublishFilter;
typedef Bt::SolarMonitor::MessageFilter<NUMBER_OF_SENSORS,STORAGE_SIZE> MessageFilter;
typedef Bt::SolarMonitor::DisplayFilter<NUMBER_OF_SENSORS> DisplayFilter;
typedef Bt::SolarMonitor::ValidateFilter<NUMBER_OF_SENSORS> ValidateFilter;
typedef Bt::SolarMonitor::AveragingFilter<NUMBER_OF_SENSORS> AveragingFilter;
typedef Bt::SolarMonitor::ForkFilter<std::array<Bt::Sensors::INA219Reading, NUMBER_OF_SENSORS>,2> ForkFilter;

Cloud sCloud(Radio, Particle, EVENT_NAME_STATUS);
PublishFilter sPublishFilter(sCloud, EVENT_NAME_DATA);
MessageFilter sMessageFilter(std::bind(&PublishFilter::consume, &sPublishFilter, std::placeholders::_1, std::placeholders::_2));
AveragingFilter sAveragingFilter(((AVERAGE_SECONDS)/MEASURE_SLEEP), std::bind(&MessageFilter::consume,&sMessageFilter, std::placeholders::_1));
ValidateFilter sValidateFilter(std::bind(&AveragingFilter::consume, &sAveragingFilter, std::placeholders::_1));
DisplayFilter sDisplayFilter(sDisplay);
ForkFilter sForkFilter(ForkFilter::Consumers{
   std::bind(&DisplayFilter::consume, &sDisplayFilter, std::placeholders::_1),
   std::bind(&ValidateFilter::consume, &sValidateFilter, std::placeholders::_1),
});


void setCharging(bool enable);

void setup() {
   BT_CORE_LOG_INFO("*** bt-solar-monitor 2 ***");
   //RGB.control(true);
   //RGB.color(0, 0, 0);
   setCharging(false);

   Wire.setSpeed(CLOCK_SPEED_100KHZ);
   Wire.begin();

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

   sDisplay.begin();
   sDisplay.setContrast(55); // Pretty good value, play around with it
   sDisplay.updateDisplay(); // with displayMap untouched, SFE logo

#if PLATFORM_ID == 10
   BT_CORE_LOG_INFO("!!! FuelGauge.sleep()  !!!");
   FuelGauge().sleep();
   BT_CORE_LOG_INFO("!!! FuelGauge DONE  !!!");
#endif

   sCloud.executeConnected([](Cloud::Client& client){
      bool ack = client.publish(EVENT_NAME_STATUS, "startup", WITH_ACK);
      BT_CORE_LOG_INFO(" ... cloud.publish(\"startup\") %d", ack);
   });


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
   sForkFilter.consume(readings);
   timer = millis() - timer;
   //BT_CORE_LOG_INFO("go to sleep after loop %u took %d ms", sLoopCounter, timer);
   //BT_CORE_LOG_WARN("%lu ms",timer);
   Serial1.flush();
   digitalWrite(sBlueLed, LOW);
   //delay(MEASURE_SLEEP * 1000);
   //Bt::Core::msSleep(500);
   System.sleep(A0, RISING, MEASURE_SLEEP);
   //System.sleep(SLEEP_MODE_SOFTPOWEROFF, MEASURE_SLEEP);
}

void setCharging(bool enable) {
#if PLATFORM_ID == 10
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
#endif
}

