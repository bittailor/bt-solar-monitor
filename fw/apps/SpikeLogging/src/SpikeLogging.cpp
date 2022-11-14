#include <Particle.h>


SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""
#define EVENT_NAME_DATA "solar-dev/data"
#define EVENT_NAME_STATUS "solar-dev/status"

STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));

Serial1LogHandler sLogHandler(115200, LOG_LEVEL_INFO);

const pin_t BLUE_LED = D7;

int sCounter = 0;

// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(BLUE_LED, OUTPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  Log.info("=> %d", sCounter++);
  digitalWrite(BLUE_LED, sCounter%2);
  delay(1s);
}
