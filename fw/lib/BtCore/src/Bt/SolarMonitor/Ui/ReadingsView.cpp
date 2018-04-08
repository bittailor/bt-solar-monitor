//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::ReadingsView
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Ui/ReadingsView.h"

#include <Bt/SolarMonitor/Ui/Icons.h>

namespace Bt {
namespace SolarMonitor {
namespace Ui {

ReadingsView::ReadingsView(Display& pDisplay)
: mDisplay(pDisplay), mCounter(0) {

}

ReadingsView::~ReadingsView() {

}

void ReadingsView::update(const Reader::Readings& pReadings) {
   mReadings = pReadings;
   mDisplay.clearDisplay();

   const uint8_t readingLookup[] = {0, 3, 4};
   const char unitLookup[] = {'W', 'V', 'A'};
   Icon* iconLookup[] = {&Icons::Sun, &Icons::Battery, &Icons::Light};


   uint8_t index = (mCounter % 3);
   uint8_t reading = readingLookup[index];

   drawIcon(*iconLookup[index], index, 0);

//   drawIcon(Icons::Sun, 0, index == 0);
//   drawIcon(Icons::Battery, 1, index == 1);
//   drawIcon(Icons::Light, 2, index == 2);

   mDisplay.drawLine(0, 15, LCDWIDTH, 15, 1);

   drawReading(1, 'A', pReadings[reading].value(), unitLookup[index]);
   drawReading(2, 'B', pReadings[reading + 5].value(), unitLookup[index]);

   mDisplay.display();
   mCounter++;
}

void ReadingsView::drawIcon(Icon& pIcon, uint8_t pPosition, bool pActive) {
//   mDisplay.fillRoundRect(pPosition*16,0,16,16,1,pActive);
//   pIcon.draw(mDisplay,pPosition*16,0,!pActive);
   if (pActive) {
      //mDisplay.drawRoundRect(pPosition*20,0,20,17,1,1);
      //mDisplay.drawRoundRect(pPosition*20+1,1,19,16,1,1);
   }
   pIcon.draw(mDisplay, pPosition*20, 0, 1);
}

void ReadingsView::drawReading(int16_t pLine ,char pPanel, float pValue, char pUnit) {
   static const size_t BUFFER_SIZE = 10;
   char buffer[BUFFER_SIZE] = {0};
   snprintf(buffer, BUFFER_SIZE, "%4.1f", pValue);

   mDisplay.setTextSize(2);
   mDisplay.setCursor(0, pLine*(2*8)+2);
   mDisplay.print(pPanel);
   mDisplay.setCursor((2*6) + 6, pLine*(2*8)+2);
   mDisplay.print(buffer);
   mDisplay.setCursor(LCDWIDTH - (2*6), pLine*(2*8)+2);
   mDisplay.print(pUnit);
}




} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt
