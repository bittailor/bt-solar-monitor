#include <functional>

#include <Particle.h>

#include <Bt/Core/Log.h>
#include <Bt/Core/PeriodicCallback.h>
#include <Bt/Core/Singleton.h>
#include <Bt/Core/Time.h>
#include <Bt/Core/Workcycle.h>
#include <Bt/SolarMonitor/LogHandler.h>

// ==== <Configuration> ==========

#define MEASURE_SLEEP 10 * 60

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

#define EVENT_NAME_DATA "dev/cloud/data"
#define EVENT_NAME_STATUS "dev/cloud/status"

// ==== </Configuration> ==========

#if Wiring_Cellular
STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));
#endif

#if Wiring_Cellular
   #define Radio Cellular
#else
   #define Radio WiFi
#endif   

SYSTEM_THREAD(ENABLED);

class NeverSleep : public Bt::Core::I_Runnable {
   public:
      virtual Bt::Core::Scheduling workcycle(){
         return Bt::Core::Scheduling::immediately();
      }
};

void send();

Bt::SolarMonitor::LogHandler sLogHandler(LOG_LEVEL_INFO,[](LogLevel pLevel){ return new Serial1LogHandler(115200, pLevel);});

Bt::Core::Time sTime;
Bt::Core::Singleton<Bt::Core::I_Time>::Instance sTimeInstance(sTime);

Bt::Core::Workcycle sWorkcycle(A0);

NeverSleep sNeverSleep;

Bt::Core::PeriodicCallback sMeasureLoop(
         Bt::Core::PeriodicCallback::SECONDS,
         10,
         [](){
            Log.info("-measure");   
         }
);

Bt::Core::PeriodicCallback sSendLoop(
         Bt::Core::PeriodicCallback::SECONDS,
         60*15,&send
);

void setup() {
   // 1*60 => OK
   // 2*60 => OK
   // 3*60 => OK
   // 4*60 => FAIL
   // 5*60 => Fail
   Particle.keepAlive(60*3);
   System.on(cloud_status, [](system_event_t event, int param){
      Log.info("System.on cloud_status %d", param);
      if(param == cloud_status_connected){
         send();
      }   
   });
   Log.info("*** Spike Data Usage ***");
   sWorkcycle.add(sMeasureLoop); 
   sWorkcycle.add(sNeverSleep);
   sWorkcycle.add(sSendLoop);
   sWorkcycle.begin();
   send();
}

void loop() {
   sWorkcycle.oneWorkcycle();
}

void send() {
   Log.info("--send");
   Particle.publish("dev/cloud/log", String::format("strength: %.02f%%", Radio.RSSI().getStrength())); 
}

