#include <functional>
#include <Particle.h>
#include <Serial4/Serial4.h>
#include <Serial5/Serial5.h>

#if PLATFORM_ID == 10
#include <cellular_hal.h>
#endif

#include <Bt/Core/Log.h>
#include <Bt/Core/Workcycle.h>
#include <Bt/Core/PeriodicCallback.h>
#include <Bt/Core/Time.h>
#include <Bt/Core/Singleton.h>
#include <Bt/Drivers/PowerManagment.h>
#include <Bt/Net/Cloud.h>
#include <Bt/SolarMonitor/AveragingFilter.h>
#include <Bt/SolarMonitor/StorageFilter.h>
#include <Bt/SolarMonitor/MessageFilter.h>
#include <Bt/SolarMonitor/ForkFilter.h>
#include <Bt/SolarMonitor/DisplayFilter.h>
#include <Bt/SolarMonitor/ValidateFilter.h>
#include <Bt/SolarMonitor/PublishFilter.h>
#include <Bt/SolarMonitor/Reader.h>

// ==== <Configuration> ==========

#define MEASURE_SLEEP 5

const size_t AVERAGE_SECONDS = 5 *  60; // 5  * 10;  // 5 *  60;
const size_t STOARGE_SECONDS = 60 * 60; // 10 * 60;  // 60 * 60;

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

void measure();



//STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
//SYSTEM_THREAD(ENABLED);

Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);

//===
//retained uint32_t sLoopCounter = 0;
//retained uint8_t sSensorState = 0;
//===

uint32_t sLoopCounter = 0;

int sBlueLed = 7;

typedef Bt::SolarMonitor::Reader Reader;
Reader sReader(Serial4, Serial5);

// CE
// RST
// DC
// BackLight
//Nokia5110LCD::Display sDisplay(A2, D6, D5, A0);
Adafruit_PCD8544 sDisplay = Adafruit_PCD8544(A2, D5, D6);

static const size_t STORAGE_SIZE = (STOARGE_SECONDS/AVERAGE_SECONDS);

static_assert(STORAGE_SIZE == 12,"oops" );

typedef Bt::Net::Cloud<decltype(Radio),decltype(Particle)> Cloud;
typedef Bt::SolarMonitor::PublishFilter<Cloud> PublishFilter;
typedef Bt::SolarMonitor::MessageFilter<Reader::NUMBER_OF_VALUES,STORAGE_SIZE> MessageFilter;
typedef Bt::SolarMonitor::DisplayFilter<Reader::NUMBER_OF_VALUES> DisplayFilter;
typedef Bt::SolarMonitor::ValidateFilter<float, Reader::NUMBER_OF_VALUES> ValidateFilter;
typedef Bt::SolarMonitor::AveragingFilter<std::array<float, Reader::NUMBER_OF_VALUES>> AveragingFilter;
typedef Bt::SolarMonitor::ForkFilter<Reader::Readings,2> ForkFilter;

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

Bt::Core::Time sTime;
Bt::Core::Singleton<Bt::Core::I_Time>::Instance sTimeInstance(sTime);

Bt::Core::Workcycle sWorkcycle;
Bt::Core::PeriodicCallback sMeasureLoop(
         Bt::Core::PeriodicCallback::SECONDS,
         MEASURE_SLEEP,
         &measure
);


void setup() {
   BT_CORE_LOG_INFO("*** bt-solar-monitor ***");
   BT_CORE_LOG_INFO("System version: %s", System.version().c_str());

   sWorkcycle.add(sMeasureLoop);

   Bt::Drivers::PowerManagment().disableCharging();

   // RGB.control(true);
   // RGB.color(0, 0, 0);

   Wire.setSpeed(CLOCK_SPEED_100KHZ);
   Wire.begin();

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

   sDisplay.begin();
   sDisplay.setContrast(55); // Pretty good value, play around with it
   sDisplay.clearDisplay();
   sDisplay.display(); // with displayMap untouched, SFE logo

   Serial4.begin(19200);
   Serial5.begin(19200);

#if PLATFORM_ID == 10
   BT_CORE_LOG_INFO("!!! FuelGauge.sleep()  !!!");
   FuelGauge().sleep();
   BT_CORE_LOG_INFO("!!! FuelGauge DONE  !!!");
#endif

   sCloud.executeConnected([](Cloud::Client& client){
      bool ack = client.publish(EVENT_NAME_STATUS, "startup", WITH_ACK);
      BT_CORE_LOG_INFO(" ... cloud.publish(\"startup\") %d", ack);
   });
}

void loop() {
   BT_CORE_LOG_INFO("-- loop memory %lu", System.freeMemory());
   sWorkcycle.oneWorkcycle();
}

void measure() {
   sLoopCounter++;
   BT_CORE_LOG_INFO("-- measure --");
   auto readings = sReader.read();
   sForkFilter.consume(readings);
}

//void loop() {
//   unsigned long timer = millis();
//   digitalWrite(sBlueLed, HIGH);
//   sLoopCounter++;
//   //BT_CORE_LOG_INFO("loop a %u [" __DATE__ " " __TIME__ "]", sLoopCounter );
//   //if(sLoopCounter%5 == 0) {
//      auto readings = sReader.read();
//      sForkFilter.consume(readings);
//   //}
//   timer = millis() - timer;
//   //BT_CORE_LOG_INFO("go to sleep after loop %u took %d ms", sLoopCounter, timer);
//   BT_CORE_LOG_DEBUG("loop %lu took %lu ms", sLoopCounter++, timer);
//   Serial1.flush();
//   digitalWrite(sBlueLed, LOW);
//   //delay(MEASURE_SLEEP * 1000);
//   //Bt::Core::msSleep(500);
//   System.sleep(A0, RISING, MEASURE_SLEEP);
//   //System.sleep(SLEEP_MODE_SOFTPOWEROFF, MEASURE_SLEEP);
//}
