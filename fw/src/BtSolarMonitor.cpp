#include <Particle.h>
#include <cellular_hal.h>
#include <nokia-5110-lcd.h>
#include <Bt/Sensors/INA219.h>
#include <Bt/Sensors/SensorArray.h>
#include <Bt/Core/Log.h>

#define APN       "gprs.swisscom.ch"
#define USERNAME  ""
#define PASSWORD  ""

SYSTEM_MODE(MANUAL);
STARTUP(cellular_credentials_set(APN, USERNAME, PASSWORD, NULL));

//STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
//SYSTEM_THREAD(ENABLED);

Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);

//===
//retained uint32_t sLoopCounter = 0;
//retained uint8_t sSensorState = 0;
//===

uint32_t sLoopCounter = 0;

int sBlueLed = 7;

uint8_t sAddresses[] = {
         0x40,
         0x44,
         0x41,
         0x45,
         0x4C,
         0x48
};

static const size_t NUMBER_OF_SENSORS = sizeof(sAddresses)/sizeof(sAddresses[0]);

std::array<Bt::Sensors::INA219, NUMBER_OF_SENSORS> sSensors{{
   {Wire, sAddresses[0], Bt::Sensors::INA219::Gain::GAIN_40MV, 32768,250},
   {Wire, sAddresses[1], Bt::Sensors::INA219::Gain::GAIN_40MV, 32768,250},
   {Wire, sAddresses[2], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
   {Wire, sAddresses[3], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
   {Wire, sAddresses[4], Bt::Sensors::INA219::Gain::GAIN_80MV, 16384,500},
   {Wire, sAddresses[5], Bt::Sensors::INA219::Gain::GAIN_80MV,  8192, 50}
}};

Bt::Sensors::SensorArray<Bt::Sensors::INA219,Bt::Sensors::INA219Reading,NUMBER_OF_SENSORS> sSensorArray{sSensors,3};


// CE
// RST
// DC
// BackLight

void setCharging(bool enable);
void tryPublish();

Nokia5110LCD::Display sDisplay(A2, D6, D5, A0);

FuelGauge sFuelGauge;

void setup() {
   BT_CORE_LOG_INFO("*** bt-solar-monitor***");
   setCharging(false);
   Wire.setSpeed(CLOCK_SPEED_100KHZ);
   Wire.begin();
   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);
   sDisplay.begin();
   sDisplay.setContrast(55); // Pretty good value, play around with it
   sDisplay.updateDisplay(); // with displayMap untouched, SFE logo

   BT_CORE_LOG_INFO("!!! Cellular.on()  !!!");
   Cellular.on();
   BT_CORE_LOG_INFO("!!! Cellular.off()  !!!");
   Cellular.off();
   BT_CORE_LOG_INFO("!!! Cellular DONE  !!!");

   BT_CORE_LOG_INFO("!!! FuelGauge.sleep()  !!!");
   sFuelGauge.sleep();
   BT_CORE_LOG_INFO("!!! FuelGauge DONE  !!!");

   for (Bt::Sensors::INA219& sensor : sSensors) {
      sensor.begin();
   }
}

void loop() {
   digitalWrite(sBlueLed, HIGH);
   unsigned long timer = millis();
   sLoopCounter++;
   //BT_CORE_LOG_INFO("loop a %u [" __DATE__ " " __TIME__ "]", sLoopCounter );
   auto readings = sSensorArray.readAll();
   sDisplay.clearDisplay();
   sDisplay.setStr(String::format("Loop %d",sLoopCounter), 0, 0, BLACK);
   for(std::size_t i = 0 ; i < readings.size() ; i++) {
      Bt::Sensors::INA219Reading& reading = readings[i];
      //BT_CORE_LOG_INFO("Reading - [%s] U = %f I = %f ", reading.valid ? "valid" : "invalid"  ,reading.busVoltage, reading.current);
      sDisplay.setStr(String::format("%.2f %.3f",reading.busVoltage,reading.current), 0, (i+1)*8, BLACK);
   }
   sDisplay.updateDisplay();
   timer = millis() - timer;
   //BT_CORE_LOG_INFO("go to sleep after loop %u took %d ms", sLoopCounter, timer);
   //BT_CORE_LOG_WARN("%d ms",timer);

   if((sLoopCounter % 10) == 9) {
      tryPublish();
   }

   Serial1.flush();
   digitalWrite(sBlueLed, LOW);
   System.sleep(A0,RISING,2);
   //System.sleep(SLEEP_MODE_SOFTPOWEROFF,2);
}

void setCharging(bool enable) {

   PMIC pmic;

   // DisableCharging turns of charging. DisableBATFET completely disconnects the battery.
   if (enable) {
      pmic.enableCharging();
      pmic.enableBATFET();
   }
   else {
      pmic.disableCharging();
      pmic.disableBATFET();
   }

   // Disabling the watchdog is necessary, otherwise it will kick in and turn
   // charing at random times, even when sleeping.

   // This disables both the watchdog and the charge safety timer in
   // Charge Termination/Timer Control Register REG05
   // pmic.disableWatchdog() disables the watchdog, but doesn't disable the
   // charge safety timer, so the red LED will start blinking slowly after
   // 1 hour if you don't do both.
   byte DATA = pmic.readChargeTermRegister();

   if (enable) {
      DATA |= 0b00111000;
   }
   else {
      // 0b11001110 = disable watchdog
      // 0b11000110 = disable watchdog and charge safety timer
      DATA &= 0b11000110;
   }

   // This would be easier if pmic.writeRegister wasn't private (or disable
   // charge safety timer had an exposed method
   Wire3.beginTransmission(PMIC_ADDRESS);
   Wire3.write(CHARGE_TIMER_CONTROL_REGISTER);
   Wire3.write(DATA);
   Wire3.endTransmission(true);

}

void tryPublish() {
   BT_CORE_LOG_INFO("Cellular.on() ...");
   Cellular.on();
   BT_CORE_LOG_INFO("... Cellular.on() DONE");
   BT_CORE_LOG_INFO("Cellular.connect() ...");
   Cellular.connect();
   BT_CORE_LOG_INFO("... Cellular.connect() DONE");
   while(! Cellular.ready() &&  Cellular.connecting()) {
      BT_CORE_LOG_INFO("Check - Cellular.ready()");
      Particle.process();
   }
   Particle.process();
   CellularSignal sig = Cellular.RSSI();
   BT_CORE_LOG_INFO("RSSI %d -dBm QUAL %d dB ", sig.rssi, sig.qual);
   TCPClient client;
   BT_CORE_LOG_INFO("connecting...");
   String loopMsg = String::format("%d",sLoopCounter);

   //const char* host = "broker.shiftr.io";
   const char* host = "solar.bockmattli.ch";


   if (client.connect(host, 80))
   {
      BT_CORE_LOG_INFO("... connecting DONE");
//      client.print("POST /electron/2/loop HTTP/1.0\r\n");
//      client.print("Host: ");client.print(host);client.print("\r\n");
//      client.print("Authorization: Basic YnQtc29sYXItZGV2aWNlOjcxOTg2YWJlODI5NjcyMTA=\r\n");
//      client.print("Content-Length: ");client.print(loopMsg.length());client.print("\r\n");
//      client.print("\r\n");
//      client.print(loopMsg);
//      client.print("\r\n");
//      client.flush();

      client.println("GET /hello/electron HTTP/1.0"); // 8 bytes
      client.print("Host: ");client.println(host);
      client.println("Content-Length: 0");
      client.println();

   }
   else
   {
      BT_CORE_LOG_INFO("connection failed");
   }
   BT_CORE_LOG_INFO("<Message>");
   while (client.available() || client.connected())
   {
      int available = client.available();
      BT_CORE_LOG_DEBUG("available == %d", available);
      if (available)
      {
         char c = client.read();
         Serial1.print(c);
      }

      if (!client.connected())
      {
         Serial1.println();
         BT_CORE_LOG_INFO("disconnecting.");
         Serial1.println();
      }
   }
   BT_CORE_LOG_INFO("</Message>");
   client.stop();
   client.flush();
   BT_CORE_LOG_INFO("Cellular.disconnect() ...");
   Cellular.disconnect();
   BT_CORE_LOG_INFO("... Cellular.disconnect() DONE");
   BT_CORE_LOG_INFO("Cellular.off() ...");
   Cellular.off();
   BT_CORE_LOG_INFO("... Cellular.off() DONE");
}


