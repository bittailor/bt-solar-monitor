#include <functional>
#include <Particle.h>

using namespace std::chrono_literals;

SYSTEM_THREAD(ENABLED);

Serial1LogHandler sSerialLogHandler(LOG_LEVEL_ALL);

void setup() {
   Log.info("*** Hello Particle ***");
   Particle.publishVitals(2min);
}


void loop() {
   Log.info("--loop--");
   Particle.publish("bt/dev/cloud/log", "Hello Particle", WITH_ACK);
   delay(5000);
}

