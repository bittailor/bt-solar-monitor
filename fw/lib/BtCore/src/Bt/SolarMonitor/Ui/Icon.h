//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::Icon
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Ui_Icon__h
#define INC__Bt_SolarMonitor_Ui_Icon__h

#include <stdint.h>
#include <Bt/SolarMonitor/Ui/Display.h>

namespace Bt {
namespace SolarMonitor {
namespace Ui {

class Icon
{
   public:
      Icon(const unsigned char* pArray, int16_t pWidth, int16_t pHeight);
      Icon(const Icon&) = delete;
      Icon& operator=(const Icon&) = delete;
      ~Icon();

      void draw(Display& pDisplay,int16_t pX, int16_t pY, uint16_t pColor);

   private:
      const unsigned char* mArray;
      int16_t mWidth;
      int16_t mHeight;
};




} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Ui_Icon__h
