#include <functional>
#include <Particle.h>

#include <Bt/Core/Log.h>

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

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

int sBlueLed = 7;

//Serial1LogHandler logHandler(115200,LOG_LEVEL_ALL);

Bt::SolarMonitor::LogHandler sLogHandler;

void setup() {
   BT_CORE_LOG_INFO("*** BUILD UP ***");

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

}

bool state = false;

void loop() {
   BT_CORE_LOG_INFO(" -LOOP- ");
   delay(1000);
}

