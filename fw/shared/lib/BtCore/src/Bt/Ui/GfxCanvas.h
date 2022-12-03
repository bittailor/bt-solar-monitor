//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::GfxCanvas
//
//*************************************************************************************************

#ifndef INC__Bt_Ui_GfxCanvas__h
#define INC__Bt_Ui_GfxCanvas__h

#include <Adafruit_GFX.h>

#include <Bt/Ui/I_Canvas.h>

namespace Bt {
namespace Ui {

class GfxCanvas : public I_Canvas
{
   public:
      GfxCanvas(Adafruit_GFX& pGfx);
      GfxCanvas(const GfxCanvas&) = delete;
      GfxCanvas& operator=(const GfxCanvas&) = delete;
      ~GfxCanvas();

      virtual void setTextColor(const Color& pColor) {
         mGfx.setTextColor(toGdxColor(pColor));
      }

      virtual void setCursor(int16_t pX, int16_t pY) {
         mGfx.setCursor(pX,pY);   
      }

      virtual size_t printf(const char* pFormat, ...) __attribute__ ((format(printf, 2, 3)));
      virtual size_t vprintf(bool newline, const char* format, va_list args) __attribute__ ((format(printf, 3, 0)));

   private:
      Adafruit_GFX& mGfx;
      
      uint16_t toGdxColor(const Color& pColor) {
         return (((pColor.red() & 0xF8) << 8) | ((pColor.green() & 0xFC) << 3) | (pColor.blue() >> 3)); 
      }

};

} // namespace Ui
} // namespace Bt

#endif // INC__Bt_Ui_GfxCanvas__h

