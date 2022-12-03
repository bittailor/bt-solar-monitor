//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::SolarChargerView
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Ui/SolarChargerView.h"

namespace Bt {
namespace SolarMonitor {
namespace Ui {

SolarChargerView::SolarChargerView() {

}

SolarChargerView::~SolarChargerView() {

}

void SolarChargerView::render(::Bt::Ui::I_Canvas& pCanvas) {
    pCanvas.setCursor(20,20);
    
    for (size_t i = 0; i < mReadings.size()/2; i++)
    {
        pCanvas.setCursor(20,22+(i*20));
        pCanvas.printf("> %5.2f => %6.3f",mReadings[i*2].value(),mReadings[(i*2)+1].value()) ;
    }
}

void SolarChargerView::update(Bt::SolarMonitor::Reader::Readings& pReadings) {
    mReadings = pReadings;
}


} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt

