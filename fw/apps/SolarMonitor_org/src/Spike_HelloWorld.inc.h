#include <functional>

#include <Particle.h>

#include <Bt/SolarMonitor/LogHandler.h>

// ==== <Configuration> ==========

#define MEASURE_SLEEP 10 * 60

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

#define EVENT_NAME_DATA "dev/cloud/data"
#define EVENT_NAME_STATUS "dev/cloud/status"

// ==== </Configuration> ==========

STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));

SYSTEM_THREAD(ENABLED);

//Serial1LogHandler logHandler(115200,LOG_LEVEL_ALL);

Bt::SolarMonitor::LogHandler sLogHandler;

int sBlueLed = 7;


void setup() {
   Log.info("*** Hello World ***");

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

}

bool state = false;

void loop() {
   Log.info(" hello world");
   Particle.publish("dev/cloud/log", "Hello World", WITH_ACK);
   delay(5000);
}

