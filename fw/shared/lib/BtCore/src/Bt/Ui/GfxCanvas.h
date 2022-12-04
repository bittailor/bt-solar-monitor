//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::GfxCanvas
//
//*************************************************************************************************

#ifndef INC__Bt_Ui_GfxCanvas__h
#define INC__Bt_Ui_GfxCanvas__h

#include <cmath>

#include <Adafruit_GFX.h>

#include <Bt/Ui/I_Canvas.h>
#include <Bt/Ui/Color.h>

namespace Bt {
namespace Ui {

class GfxCanvas : public I_Canvas
{
   public:
      GfxCanvas(Adafruit_GFX& pGfx);
      GfxCanvas(const GfxCanvas&) = delete;
      GfxCanvas& operator=(const GfxCanvas&) = delete;
      ~GfxCanvas();

      virtual int16_t width() const { return mGfx.width(); }
      virtual int16_t height() const { return mGfx.height(); }

      virtual void setTextSize(uint8_t pSize) {
         mGfx.setTextSize(pSize);   
      }

      virtual void setTextColor(const Color& pColor) {
         mGfx.setTextColor(toGdxColor(pColor));
      }

      virtual void setCursor(int16_t pX, int16_t pY) {
         mGfx.setCursor(pX,pY);   
      }

      virtual size_t printf(const char* pFormat, ...) __attribute__ ((format(printf, 2, 3)));
      virtual size_t vprintf(bool newline, const char* format, va_list args) __attribute__ ((format(printf, 3, 0)));

      virtual void fillRect(int16_t pX, int16_t pY, int16_t pWidth, int16_t pHeight, const Color& pColor) {
         mGfx.fillRect(pX , pY, pWidth, pHeight, toGdxColor(pColor)); 
      }


      virtual void drawCircle(int16_t pX, int16_t pY, int16_t pR, const Color& pColor) {
         mGfx.drawCircle(pX , pY, pR, toGdxColor(pColor));   
      }



      virtual void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, const Color& pColor) {

         byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
         byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

         // Calculate first pair of coordinates for segment start
         float sx = cos((start_angle - 90) * DEG2RAD);
         float sy = sin((start_angle - 90) * DEG2RAD);
         uint16_t x0 = sx * (rx - w) + x;
         uint16_t y0 = sy * (ry - w) + y;
         uint16_t x1 = sx * rx + x;
         uint16_t y1 = sy * ry + y;

         // Draw colour blocks every inc degrees
         for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

            // Calculate pair of coordinates for segment end
            float sx2 = cos((i + seg - 90) * DEG2RAD);
            float sy2 = sin((i + seg - 90) * DEG2RAD);
            int x2 = sx2 * (rx - w) + x;
            int y2 = sy2 * (ry - w) + y;
            int x3 = sx2 * rx + x;
            int y3 = sy2 * ry + y;

            mGfx.fillTriangle(x0, y0, x1, y1, x2, y2, toGdxColor(pColor));
            mGfx.fillTriangle(x1, y1, x2, y2, x3, y3, toGdxColor(pColor));

            // Copy segment end to sgement start for next segment
            x0 = x2;
            y0 = y2;
            x1 = x3;
            y1 = y3;
         }
      }

      virtual void  drawXBitmap(int16_t pX, int16_t  pY, const uint8_t pBitmap[], int16_t pWidth, int16_t pHeight, const Color& pColor) {
         mGfx.drawXBitmap(pX, pY, pBitmap, pWidth, pHeight, toGdxColor(pColor));    
      }

   private:
      static constexpr float DEG2RAD = 0.0174532925;

      Adafruit_GFX& mGfx;
      
      uint16_t toGdxColor(const Color& pColor) {
         return (((pColor.red() & 0xF8) << 8) | ((pColor.green() & 0xFC) << 3) | (pColor.blue() >> 3)); 
      }

};

} // namespace Ui
} // namespace Bt

#endif // INC__Bt_Ui_GfxCanvas__h

