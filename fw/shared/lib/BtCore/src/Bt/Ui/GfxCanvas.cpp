//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::GfxCanvas
//
//*************************************************************************************************

#include "Bt/Ui/GfxCanvas.h"

#include <stdarg.h>

namespace Bt {
namespace Ui {

GfxCanvas::GfxCanvas(Adafruit_GFX& pGfx):mGfx(pGfx)  {

}

GfxCanvas::~GfxCanvas() {

}

size_t GfxCanvas::printf(const char* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    auto r = this->vprintf(false, pFormat, args);
    va_end(args);
    return r;   
}

size_t GfxCanvas::vprintf(bool newline, const char* format, va_list args) {
    return mGfx.vprintf(newline, format, args);   
}

} // namespace Ui
} // namespace Bt

