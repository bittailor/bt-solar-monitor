//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::CanvasWrapper
//
//*************************************************************************************************

#ifndef INC__Bt_Ui_CanvasWrapper__h
#define INC__Bt_Ui_CanvasWrapper__h

#include "Bt/Ui/I_Canvas.h"

namespace Bt {
namespace Ui {

class CanvasWrapper : public I_Canvas
{
   public:
      CanvasWrapper(I_Canvas& pCanvas, int16_t pOffsetX, int16_t pOffsetY)
      :mCanvas(pCanvas), mOffsetX(pOffsetX), mOffsetY(pOffsetY) {
      }

      virtual int16_t width() const { return mCanvas.width() - mOffsetX  ;}
      virtual int16_t height() const { return mCanvas.height() - mOffsetY ;}

      virtual void setTextSize(uint8_t pSize) {
         mCanvas.setTextSize(pSize);   
      } 
      virtual void setTextColor(const Color& pColor) {
         mCanvas.setTextColor(pColor);      
      }
      virtual void setCursor(int16_t pX, int16_t pY) {
         mCanvas.setCursor(pX+mOffsetX, pY+mOffsetY);   
      }

      virtual size_t printf(const char* pFormat, ...) __attribute__ ((format(printf, 2, 3)));
      virtual size_t vprintf(bool newline, const char* format, va_list args) __attribute__ ((format(printf, 3, 0)));

      virtual void drawCircle(int16_t pX, int16_t pY, int16_t pR, const Color& pColor) {
         mCanvas.drawCircle(pX+mOffsetX, pY+mOffsetY, pR, pColor);   
      }

      virtual void fillArc(int pX, int pY, int pStartAngle, int pSegCount, int pRadiusX, int pRadiusY, int pWidth, const Color& pColor) {
         mCanvas.fillArc(pX+mOffsetX, pY+mOffsetY, pStartAngle, pSegCount, pRadiusX, pRadiusY, pWidth, pColor);
      }
      virtual void fillRect(int16_t pX, int16_t pY, int16_t pWidth, int16_t pHeight, const Color& pColor) {
         mCanvas.fillRect(pX+mOffsetX, pY+mOffsetY, pWidth, pHeight, pColor);   
      }

      virtual void  drawXBitmap(int16_t pX, int16_t  pY, const uint8_t pBitmap[], int16_t pWidth, int16_t pHeight, const Color& pColor) {
         mCanvas.drawXBitmap(pX+mOffsetX, pY+mOffsetY, pBitmap, pWidth, pHeight, pColor);    
      }

   private:
      I_Canvas& mCanvas;
      int16_t mOffsetX;
      int16_t mOffsetY;
      
};

} // namespace Ui
} // namespace Bt

#endif // INC__Bt_Ui_CanvasWrapper__h

