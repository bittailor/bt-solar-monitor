//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::Icon
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Ui/Icon.h"

namespace Bt {
namespace SolarMonitor {
namespace Ui {

Icon::Icon(const unsigned char* pArray, int16_t pWidth, int16_t pHeight)
: mArray(pArray), mWidth(pWidth), mHeight(pHeight) {

}

Icon::~Icon() {

}

void Icon::draw(Display& pDisplay,int16_t pX, int16_t pY, uint16_t pColor) {
   pDisplay.drawXBitmap(pX, pY, mArray , mWidth, mHeight, pColor);
}

} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt
