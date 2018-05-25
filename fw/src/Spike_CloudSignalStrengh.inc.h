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

#define MEASURE_SLEEP 10 * 60

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

#define EVENT_NAME_DATA "dev/cloud/data"
#define EVENT_NAME_STATUS "dev/cloud/status"

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

SYSTEM_THREAD(ENABLED);

void measure();

Bt::SolarMonitor::LogHandler sLogHandler;

typedef Bt::Net::Cloud<decltype(Radio),decltype(Particle)> Cloud;

Bt::Core::Time sTime;
Bt::Core::Singleton<Bt::Core::I_Time>::Instance sTimeInstance(sTime);

Cloud sCloud(Radio, Particle, EVENT_NAME_STATUS);

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
   sLogHandler.changeLevel(LogLevel::INFO_LEVEL);
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

#if PLATFORM_ID == 10
   BT_CORE_LOG_INFO("!!! FuelGauge.sleep()  !!!");
   FuelGauge().sleep();
   BT_CORE_LOG_INFO("!!! FuelGauge DONE  !!!");
#endif

   sCloud.executeConnected([](Cloud::Client& client){
      bool ack = client.publish(EVENT_NAME_STATUS, "startup", WITH_ACK);
      BT_CORE_LOG_INFO(" ... cloud.publish(\"startup\") %d", ack);
   });
   BT_CORE_LOG_INFO("setup done => wait for cloud.publish(\"startup\")");
}

void loop() {
   BT_CORE_LOG_DEBUG("-- loop memory %lu", System.freeMemory());
   sWorkcycle.oneWorkcycle();
}

void measure() {
   sCloud.executeConnected([](Cloud::Client& client){
      bool ack = client.publish(EVENT_NAME_STATUS, "b measure", WITH_ACK);
      double sumRssi = 0;
      double sumQual = 0;
      for(int i=0 ; i < 60 ; i++) {
         auto signal = Radio.RSSI();
         BT_CORE_LOG_INFO("RSSI |%d|%d", signal.rssi, signal.qual);
         sumRssi += signal.rssi;
         sumQual += signal.qual;
         delay(1000);
      }
      BT_CORE_LOG_INFO("Mean RSSI |%d|%d", int(sumRssi/60), int(sumQual/60));

      ack = client.publish(EVENT_NAME_STATUS, "e measure", WITH_ACK);
   });
}
