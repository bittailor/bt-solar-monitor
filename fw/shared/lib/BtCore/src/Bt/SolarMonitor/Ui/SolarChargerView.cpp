//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::SolarChargerView
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Ui/SolarChargerView.h"

#include <Bt/Ui/CanvasWrapper.h>

namespace Bt {
namespace SolarMonitor {
namespace Ui {

SolarChargerView::SolarChargerView() {

}

SolarChargerView::~SolarChargerView() {

}

void SolarChargerView::render(::Bt::Ui::I_Canvas& pCanvas) {
    auto readingA = SolarReading::unpack(mReadings,SolarReading::A);
    auto readingB = SolarReading::unpack(mReadings,SolarReading::B);
    auto xOffset = 14;
    {
        pCanvas.fillRect(0, 0, pCanvas.width() , 19, Bt::Ui::BLACK);
        pCanvas.setTextSize(2);
        pCanvas.setCursor(6,2);
        pCanvas.setTextColor(Bt::Ui::WHITE);
        pCanvas.printf("Bockmattli");
        pCanvas.setTextColor(Bt::Ui::BLACK);
        auto canvas = Bt::Ui::CanvasWrapper(pCanvas,xOffset,21);
        renderSolarReading(canvas, readingA);
    }
    {
        auto yOffset = 138;
        pCanvas.fillRect(0, yOffset, pCanvas.width() , 19, Bt::Ui::BLACK);
        pCanvas.setTextSize(2);
        pCanvas.setCursor(17,yOffset+2);
        pCanvas.setTextColor(Bt::Ui::WHITE);
        pCanvas.printf("Fluebrig");
        pCanvas.setTextColor(Bt::Ui::BLACK);
        auto canvas = Bt::Ui::CanvasWrapper(pCanvas,xOffset,yOffset+21);
        renderSolarReading(canvas, readingB);
    }
    pCanvas.fillRect(0, pCanvas.height()-19, pCanvas.width() , 19, Bt::Ui::BLACK);
    pCanvas.setTextSize(1);
    pCanvas.setTextColor(Bt::Ui::WHITE);
    pCanvas.setCursor(49,pCanvas.height()-17);
    pCanvas.printf("%2d:%02d", Time.hour(), Time.minute());
    pCanvas.setCursor(31,pCanvas.height()-8);
    pCanvas.printf("%02d/%02d/%04d", Time.day(), Time.month(), Time.year());
}

void SolarChargerView::update(Bt::SolarMonitor::Reader::Readings& pReadings) {
    mReadings = pReadings;
}

void SolarChargerView::renderSolarReading(::Bt::Ui::I_Canvas& pCanvas, SolarReading& pSolarReading) {
    pCanvas.fillArc(50, 50, 240 , 80 , 50, 50 , 10,  Bt::Ui::BLACK);
    pCanvas.fillArc(50, 50, 243 , 78 , 48, 48 , 6,   Bt::Ui::WHITE);
    pCanvas.fillArc(50, 50, 240 , pSolarReading.panelPower().value() , 47, 47 , 4,   Bt::Ui::BLACK);

    pCanvas.setTextSize(2);
    pCanvas.setCursor(45,20);
    pCanvas.printf("W");
    pCanvas.setTextSize(3);
    pCanvas.setCursor(16,40);
    pCanvas.printf("%4.1f",pSolarReading.panelPower().value());

    pCanvas.setTextSize(2);
    pCanvas.setCursor(20,80);
    pCanvas.printf("%4.1fV",pSolarReading.chargerVoltage().value());

    pCanvas.setCursor(20,100);
    pCanvas.printf("%4.1fA",pSolarReading.loadCurrent().value());    
}


} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt

