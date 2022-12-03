//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::CanvasWrapper
//
//*************************************************************************************************

#ifndef INC__Bt_Ui_CanvasWrapper__h
#define INC__Bt_Ui_CanvasWrapper__h

#include <cstdarg>

#include "Bt/Ui/I_Canvas.h"

namespace Bt {
namespace Ui {

class CanvasWrapper : public I_Canvas
{
   public:
      CanvasWrapper(I_Canvas& pCanvas, int16_t pX, int16_t pY);
      CanvasWrapper(const CanvasWrapper&) = delete;
      CanvasWrapper& operator=(const CanvasWrapper&) = delete;
      ~CanvasWrapper();

      virtual void setTextColor(const Color& pColor) {
         mCanvas.setTextColor(pColor);
      }

      virtual void setCursor(int16_t pX, int16_t pY) {
         mCanvas.setCursor(pX+mX,pY+mY);   
      }

      virtual size_t printf(const char* pFormat, ...) __attribute__ ((format(printf, 2, 3)));
      virtual size_t vprintf(bool newline, const char* format, va_list args) __attribute__ ((format(printf, 3, 0)));

   private:
      I_Canvas& mCanvas;
      int16_t mX; 
      int16_t mY;
      
};

} // namespace Ui
} // namespace Bt

#endif // INC__Bt_Ui_CanvasWrapper__h

