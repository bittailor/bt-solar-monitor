#define SOLAR_DEV 1

#include <functional>
#include <Particle.h>
#include <Serial4/Serial4.h>
#include <Serial5/Serial5.h>

#if PLATFORM_ID == 10
#include <cellular_hal.h>
#endif

#include <BtCore.h>

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
#include <Bt/SolarMonitor/MessageBufferSink.h>
#include <Bt/SolarMonitor/PublishFilter.h>
#include <Bt/SolarMonitor/LogHandler.h>
#include <Bt/SolarMonitor/Reader.h>
#include <Bt/SolarMonitor/EmulatedReader.h>
#include <Bt/SolarMonitor/Cli/CliController.h>
#include <Bt/Core/InterruptPushButton.h>

#define ENABLE_GxEPD2_GFX 0

#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>

#if defined(SOLAR_PROD)

// ==== <Configuration> ==========

#define CONFIGURATION_ENVIRONMENT "PROD"

#define MEASURE_SLEEP 5

#define PUBLISH_SLEEP 60 * 15

const size_t AVERAGE_SECONDS = 5 *  60; // 5  * 10;  // 5 *  60;

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

#define EVENT_NAME_DATA "solar/data"
#define EVENT_NAME_STATUS "solar/status"

#define RUN_LOG_LEVEL WARN_LEVEL

typedef Bt::SolarMonitor::Reader Reader;

// ==== </Configuration> ==========

#elif defined(SOLAR_DEV)

// ==== <Configuration> ==========

#define CONFIGURATION_ENVIRONMENT "DEV"

#define MEASURE_SLEEP 5 // 1 // 5 sec

#define PUBLISH_SLEEP 60 * 15    // 30 // 15 min

const size_t AVERAGE_SECONDS = 30; // 30 sec 

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

#define EVENT_NAME_DATA "solar-dev/data"
#define EVENT_NAME_STATUS "solar-dev/status"

#define RUN_LOG_LEVEL INFO_LEVEL

typedef Bt::SolarMonitor::EmulatedReader Reader;

// ==== </Configuration> ==========

#else
   #error no solar enviroment defined
#endif

SYSTEM_MODE(MANUAL);

#if PLATFORM_ID == 10
STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));
#endif

#if PLATFORM_ID == 10
   #define Radio Cellular
   #define BUTTON_LEFT D5
   #define BUTTON_RIGHT D4
   #define BUTTON_UP D2
   #define BUTTON_DOWN D3


#else
   #define Radio WiFi
   #define BUTTON_LEFT D5
   #define BUTTON_RIGHT D4
   #define BUTTON_UP D2
   #define BUTTON_DOWN D3

   class SerialStub : public Stream {
      public:
         virtual int available(){return 0;}
         virtual int read(){return 0;}
         virtual int peek(){return 0;}
         virtual void flush(){}
         virtual size_t write(uint8_t){return 0;}
         void begin(unsigned long){}
   };
   
   
   SerialStub Serial4; 
   SerialStub Serial5;
#endif

void measure();

SYSTEM_THREAD(ENABLED);

Bt::SolarMonitor::LogHandler sLogHandler(LOG_LEVEL_INFO);

int sBlueLed = 7;

Reader sReader(Serial4, Serial5);

static const size_t MESSAGE_BUFFER_SIZE = 24 * 2;
static const size_t NUMBER_OF_RECORDS_IN_A_MESSAGE = 16 ; //16;  //6;


typedef Bt::Net::Cloud<decltype(Radio),decltype(Particle)> Cloud;
typedef Bt::SolarMonitor::PublishFilter<Cloud> PublishFilter;
typedef Bt::SolarMonitor::MessageBufferSink<MESSAGE_BUFFER_SIZE> MessageBufferSink;
typedef Bt::SolarMonitor::MessageFilter<Reader::NUMBER_OF_VALUES,NUMBER_OF_RECORDS_IN_A_MESSAGE> MessageFilter;
typedef Bt::SolarMonitor::ValidateFilter<float, Reader::NUMBER_OF_VALUES> ValidateFilter;
typedef Bt::SolarMonitor::AveragingFilter<std::array<float, Reader::NUMBER_OF_VALUES>> AveragingFilter;
typedef Bt::SolarMonitor::LogFilter<Reader::NUMBER_OF_VALUES> LogFilter;

typedef Bt::SolarMonitor::ForkFilter<Reader::Readings,2> ForkFilter;

Bt::Core::Time sTime;
Bt::Core::Singleton<Bt::Core::I_Time>::Instance sTimeInstance(sTime);


Cloud sCloud(Radio, Particle);
MessageBufferSink sMessageBufferSink;
PublishFilter sPublishFilter(sCloud, EVENT_NAME_DATA, sMessageBufferSink);
MessageFilter sMessageFilter(std::bind(&MessageBufferSink::consume, &sMessageBufferSink, std::placeholders::_1));
AveragingFilter sAveragingFilter(((AVERAGE_SECONDS)/MEASURE_SLEEP), std::bind(&MessageFilter::consume,&sMessageFilter, std::placeholders::_1));
ValidateFilter sValidateFilter(std::bind(&AveragingFilter::consume, &sAveragingFilter, std::placeholders::_1));
LogFilter sLogFilter;
ForkFilter sForkFilter(ForkFilter::Consumers{
   std::bind(&LogFilter::consume, &sLogFilter, std::placeholders::_1),
   std::bind(&ValidateFilter::consume, &sValidateFilter, std::placeholders::_1),
});

Bt::Core::Workcycle sWorkcycle(A4);
Bt::Core::PeriodicCallback sMeasureLoop(
         Bt::Core::PeriodicCallback::SECONDS,
         MEASURE_SLEEP,
         &measure
);

Bt::Core::PeriodicCallback sPublishLoop(
         Bt::Core::PeriodicCallback::SECONDS,
         PUBLISH_SLEEP,
         [](){

   sPublishFilter.publish();
}
);


Bt::Core::InterruptPushButton sLeft(BUTTON_LEFT, [](){
   BT_CORE_LOG_INFO("click left");
});
Bt::Core::InterruptPushButton sRight(BUTTON_RIGHT, [](){
   BT_CORE_LOG_INFO("click right");
});
Bt::Core::InterruptPushButton sUp(BUTTON_UP, [](){
   BT_CORE_LOG_INFO("click up");
});
Bt::Core::InterruptPushButton sDown(BUTTON_DOWN, [](){
   BT_CORE_LOG_INFO("click down");
});

Bt::SolarMonitor::Cli::CliController sCliController(Serial1);

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> sDisplay(GxEPD2_290(/*CS=A2*/ SS, /*DC=*/ A1, /*RST=*/ A0, /*BUSY=*/ B5));


void setup() {
   sLogHandler.changeLevel(LogLevel::INFO_LEVEL);
   BT_CORE_LOG_INFO("*** bt-solar-monitor " CONFIGURATION_ENVIRONMENT " ***");
   BT_CORE_LOG_INFO("System version: %s", System.version().c_str());
   BT_CORE_LOG_INFO("PLATFORM_ID %d",PLATFORM_ID);

   Bt::Drivers::PowerManagment().disableCharging();

   sWorkcycle.add(sMeasureLoop);
   sWorkcycle.add(sPublishLoop);
   sWorkcycle.add(sLeft);
   sWorkcycle.add(sRight);
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
   sLeft.begin();
   sRight.begin();
   sUp.begin();
   sDown.begin();

   pinMode(B0, OUTPUT);
   digitalWrite(B0, HIGH);


   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);



   Serial4.begin(19200);
   Serial5.begin(19200);

   
   BT_CORE_LOG_INFO("=> %d",PLATFORM_ID);
#if PLATFORM_ID == 10
   BT_CORE_LOG_INFO("!!! FuelGauge.sleep()  !!!");
   FuelGauge().sleep();
   BT_CORE_LOG_INFO("!!! FuelGauge DONE  !!!");
#endif
   BT_CORE_LOG_INFO(" <= %d",PLATFORM_ID);


   sCloud.executeConnected([](Cloud::Client& client){
      bool ack = client.publish(EVENT_NAME_STATUS, "startup", WITH_ACK);
      if(!ack) {
         return false;
      }
      BT_CORE_LOG_INFO(" ... cloud.publish(\"startup\") %d", ack);
      sLogHandler.changeLevel(LogLevel::RUN_LOG_LEVEL);
      return true;
   });


   sDisplay.init(115200);
   sDisplay.setRotation(1);
   sDisplay.setTextColor(GxEPD_BLACK);
   sDisplay.fillScreen(GxEPD_WHITE);
  
   sDisplay.fillRect(0,0,sDisplay.width(),20, GxEPD_BLACK);
   sDisplay.setTextColor(GxEPD_WHITE);
   sDisplay.setCursor(1,1);
   sDisplay.setTextSize(2);
   sDisplay.print("Solar");
   sDisplay.setTextColor(GxEPD_BLACK);
   sDisplay.display();

   BT_CORE_LOG_INFO("setup done => wait for cloud.publish(\"startup\")");
}

void loop() {
   // BT_CORE_LOG_DEBUG("-- loop memory %lu", System.freeMemory());
   sWorkcycle.oneWorkcycle();
}

int sCounter = 0;

void measure() {
   auto readings = sReader.read();
   sForkFilter.consume(readings);

   sDisplay.fillRect(0, 20, sDisplay.width(), sDisplay.height()-20, GxEPD_WHITE);
   sDisplay.setTextColor(GxEPD_BLACK);
   sDisplay.setTextSize(3);
   sDisplay.setCursor(10, 80);
   sDisplay.printf("%2.2f => %2.3f", readings[0].value(),readings[1].value());
   
   sDisplay.display((sCounter++)%20 != 0);
}

