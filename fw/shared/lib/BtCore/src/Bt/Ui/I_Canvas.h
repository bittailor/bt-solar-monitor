//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Ui::I_Canvas
//
//*************************************************************************************************

#ifndef INC__Bt_Ui_I_Canvas__h
#define INC__Bt_Ui_I_Canvas__h

#include <stdio.h>

#include "Bt/Ui/Color.h"

namespace Bt {
namespace Ui {

class I_Canvas {
   public:
      virtual ~I_Canvas() {}

      virtual int16_t width() const = 0;
      virtual int16_t height() const = 0;

      virtual void setTextSize(uint8_t pSize) = 0; 
      virtual void setTextColor(const Color& pColor) = 0;
      virtual void setCursor(int16_t pX, int16_t pY) = 0;
      virtual int16_t getCursorX() const = 0;
      virtual int16_t getCursorY() const = 0;

      virtual size_t printf(const char* pFormat, ...) __attribute__ ((format(printf, 2, 3))) = 0;
      virtual size_t vprintf(bool newline, const char* format, va_list args) __attribute__ ((format(printf, 3, 0))) = 0;


      virtual void drawChar(int16_t pX, int16_t pY, unsigned char pChar, const Color& pColor, const Color& pBackground, uint8_t pSize) = 0;
      virtual void drawCircle(int16_t pX, int16_t pY, int16_t pR, const Color& pColor) = 0;

      virtual void fillRect(int16_t pX, int16_t pY, int16_t pWidth, int16_t pHeight, const Color& pColor) = 0;
      virtual void fillArc(int pX, int pY, int pStartAngle, int pSegCount, int pRadiusX, int pRadiusY, int pWidth, const Color& pColor) = 0;

      virtual void  drawXBitmap(int16_t pX, int16_t  pY, const uint8_t pBitmap[], int16_t pWidth, int16_t pHeight, const Color& pColor) = 0;

};





} // namespace Ui
} // namespace Bt

#endif // INC__Bt_Ui_I_Canvas__h

