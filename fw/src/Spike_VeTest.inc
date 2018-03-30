#include <functional>

#include <Particle.h>
#include <Serial4/Serial4.h>
#include <Serial5/Serial5.h>

#include <Bt/Sensors/VeDirect/Registers.h>

using namespace Bt::Sensors::VeDirect;

SYSTEM_MODE(MANUAL);
Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);

uint32_t sLoopCounter = 0;
int sBlueLed = 7;

int inIndex = 0;
char inBuffer[1000] = {0};
int outIndex = 0;
char outBuffer[1000] = {0};

int cmdIndex = 0;
char cmdBuffer[1000] = {0};

Stream* pInputStream = 0;

void setup() {
   Log.info("*** VE.Direct Test ***");

   Serial4.begin(19200);
   Serial5.begin(19200);

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

}


void log(Stream& pStream) {
   CommandExecutor version(pStream,":154");
   const char* rsp = version.execute();
   Log.info(" ping => %s", rsp);

   Log.info(" version %s", Version().read(pStream).value().c_str());
   Log.info(" modelName %s", Registers::modelName().get(pStream).value().c_str());
   Log.info(" serialNumber %s", Registers::serialNumber().get(pStream).value().c_str());
   Log.info(" panelVoltage %f", Registers::panelVoltage().scaled(pStream).value());
   Log.info(" panelPower %f", Registers::panelPower().scaled(pStream).value());
   Log.info(" chargerCurrent %f", Registers::chargerCurrent().scaled(pStream).value());
   Log.info(" chargerVoltage %f", Registers::chargerVoltage().scaled(pStream).value());
   Log.info(" loadCurrent %f", Registers::loadCurrent().scaled(pStream).value());
   Log.info(" loadOutputState %s", Registers::loadOutputState().get(pStream).value() ? "ON" : "OFF");
}

void loop() {
   Log.info(" ");
   Log.info("Serial 4:");
   log(Serial4);
   Log.info("Serial 5:");
   log(Serial5);
   delay(2000);
}

