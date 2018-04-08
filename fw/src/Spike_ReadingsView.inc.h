#include <Particle.h>
#include <Bt/Core/Log.h>
#include <Bt/SolarMonitor/Reader.h>
#include <Bt/SolarMonitor/Ui/ReadingsView.h>


#define ICON_SIZE 16

static const unsigned char sSunBits[] = {
   0x00, 0x00, 0x80, 0x00, 0x84, 0x10, 0x88, 0x08, 0x10, 0x04, 0x80, 0x00,
   0xc0, 0x01, 0xee, 0x3b, 0xc0, 0x01, 0x80, 0x00, 0x10, 0x04, 0x88, 0x08,
   0x84, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char sBatteryBits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x0c, 0x18, 0xfe, 0x3f,
   0xfe, 0x3f, 0x56, 0x35, 0x56, 0x35, 0x56, 0x35, 0xfe, 0x3f, 0xfe, 0x3f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char sLightBits[] = {
   0x00, 0x00, 0xc0, 0x03, 0x20, 0x04, 0x10, 0x08, 0x08, 0x10, 0x08, 0x10,
   0x08, 0x10, 0x10, 0x08, 0x20, 0x04, 0x40, 0x02, 0xc0, 0x03, 0xc0, 0x03,
   0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00
};

SYSTEM_MODE(MANUAL);

Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);
Adafruit_PCD8544 sDisplay = Adafruit_PCD8544(A2, D5, D6);
Bt::SolarMonitor::Ui::ReadingsView sReadingsView(sDisplay);

void show(int16_t line ,char panel, float value, char unit) {
   static const size_t BUFFER_SIZE = 10;
   char buffer[BUFFER_SIZE] = {0};
   snprintf(buffer, BUFFER_SIZE, "%4.1f", value);
   sDisplay.setTextSize(2);
   sDisplay.setCursor(0,line*(2*8));
   sDisplay.print(panel);
   sDisplay.setCursor((2*6) + 6,line*(2*8));
   sDisplay.print(buffer);
   sDisplay.setCursor(LCDWIDTH - (2*6),line*(2*8));
   sDisplay.print(unit);
}

void setup() {
   BT_CORE_LOG_INFO("*** " __FILE__ " ***");
   BT_CORE_LOG_INFO("System version: %s", System.version().c_str());

   sDisplay.setCursor(ICON_SIZE, 16);

   sDisplay.begin();
   sDisplay.setContrast(55); // Pretty good value, play around with it
   sDisplay.clearDisplay();
   sDisplay.display(); // with displayMap untouched, SFE logo





}

void loop() {
   Bt::SolarMonitor::Reader::Readings readings{1,2,3,4,5,6,7,8,9,10};
   sReadingsView.update(readings);
   delay(3000);

}

