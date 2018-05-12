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
#include <Bt/SolarMonitor/ValidateFilter.h>
#include <Bt/SolarMonitor/LogFilter.h>
#include <Bt/SolarMonitor/PublishFilter.h>
#include <Bt/SolarMonitor/LogHandler.h>
#include <Bt/SolarMonitor/Reader.h>
#include <Bt/SolarMonitor/Cli/CliController.h>
#include <Bt/Core/InterruptPushButton.h>

// ==== <Configuration> ==========

#define MEASURE_SLEEP 5

const size_t AVERAGE_SECONDS = 5 *  60; // 5  * 10;  // 5 *  60;
const size_t STOARGE_SECONDS = 60 * 60; // 10 * 60;  // 60 * 60;
// const size_t AVERAGE_SECONDS = 15; // 5  * 10;  // 5 *  60;
// const size_t STOARGE_SECONDS = 3 * 60; // 10 * 60;  // 60 * 60;

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

SYSTEM_THREAD(ENABLED);


Bt::SolarMonitor::LogHandler sLogHandler;

int sBlueLed = 7;

typedef Bt::SolarMonitor::Reader Reader;
Reader sReader(Serial4, Serial5);

static const size_t STORAGE_SIZE = (STOARGE_SECONDS/AVERAGE_SECONDS);

static_assert(STORAGE_SIZE == 12,"oops" );

typedef Bt::Net::Cloud<decltype(Radio),decltype(Particle)> Cloud;
typedef Bt::SolarMonitor::PublishFilter<Cloud> PublishFilter;
typedef Bt::SolarMonitor::MessageFilter<Reader::NUMBER_OF_VALUES,STORAGE_SIZE> MessageFilter;
typedef Bt::SolarMonitor::ValidateFilter<float, Reader::NUMBER_OF_VALUES> ValidateFilter;
typedef Bt::SolarMonitor::AveragingFilter<std::array<float, Reader::NUMBER_OF_VALUES>> AveragingFilter;
typedef Bt::SolarMonitor::LogFilter<Reader::NUMBER_OF_VALUES> LogFilter;

typedef Bt::SolarMonitor::ForkFilter<Reader::Readings,2> ForkFilter;

Bt::Core::Time sTime;
Bt::Core::Singleton<Bt::Core::I_Time>::Instance sTimeInstance(sTime);

Cloud sCloud(Radio, Particle, EVENT_NAME_STATUS);
PublishFilter sPublishFilter(sCloud, EVENT_NAME_DATA);
MessageFilter sMessageFilter(std::bind(&PublishFilter::consume, &sPublishFilter, std::placeholders::_1, std::placeholders::_2));
AveragingFilter sAveragingFilter(((AVERAGE_SECONDS)/MEASURE_SLEEP), std::bind(&MessageFilter::consume,&sMessageFilter, std::placeholders::_1));
ValidateFilter sValidateFilter(std::bind(&AveragingFilter::consume, &sAveragingFilter, std::placeholders::_1));
LogFilter sLogFilter;
ForkFilter sForkFilter(ForkFilter::Consumers{
   std::bind(&LogFilter::consume, &sLogFilter, std::placeholders::_1),
   std::bind(&ValidateFilter::consume, &sValidateFilter, std::placeholders::_1),
});

Bt::Core::Workcycle sWorkcycle(A0);
Bt::Core::PeriodicCallback sMeasureLoop(
         Bt::Core::PeriodicCallback::SECONDS,
         MEASURE_SLEEP,
         &measure
);

Bt::Core::InterruptPushButton sSelect(A0, [](){
   BT_CORE_LOG_INFO("click select");
});
Bt::Core::InterruptPushButton sUp(C5, [](){
   BT_CORE_LOG_INFO("click up");
});
Bt::Core::InterruptPushButton sDown(C4, [](){
   BT_CORE_LOG_INFO("click down");
});

Bt::SolarMonitor::Cli::CliController sCliController(Serial1);

void setup() {
   BT_CORE_LOG_INFO("*** bt-solar-monitor ***");
   BT_CORE_LOG_INFO("System version: %s", System.version().c_str());

   Bt::Drivers::PowerManagment().disableCharging();

   sWorkcycle.add(sMeasureLoop);
   sWorkcycle.add(sSelect);
   sWorkcycle.add(sUp);
   sWorkcycle.add(sDown);
   sWorkcycle.add(sCloud);
   sWorkcycle.add(sCliController);

   sWorkcycle.addSchedulingListener(sCliController);

   sCliController.addCommand("loglevel", [](Stream& pStream, int pArgc, char* pArgv[]){
      sLogHandler.changeLevel(pStream,pArgc,pArgv);
   });

   sCloud.begin();
   sWorkcycle.begin();
   sSelect.begin();
   sUp.begin();
   sDown.begin();

   // RGB.control(true);
   // RGB.color(0, 0, 0);

   Wire.setSpeed(CLOCK_SPEED_100KHZ);
   Wire.begin();

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

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
   BT_CORE_LOG_DEBUG("-- loop memory %lu", System.freeMemory());
   sWorkcycle.oneWorkcycle();
}

void measure() {
   auto readings = sReader.read();
   sForkFilter.consume(readings);
}
