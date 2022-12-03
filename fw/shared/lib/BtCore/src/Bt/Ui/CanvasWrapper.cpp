//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::CanvasWrapper
//
//*************************************************************************************************

#include "Bt/Ui/CanvasWrapper.h"

namespace Bt {
namespace Ui {

CanvasWrapper::CanvasWrapper(I_Canvas& pCanvas, int16_t pX, int16_t pY) 
:mCanvas(pCanvas), mX(pX), mY(pY) {

}

CanvasWrapper::~CanvasWrapper() {

}

size_t CanvasWrapper::printf(const char* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    auto r = this->vprintf(false, pFormat, args);
    va_end(args);
    return r;   
}

size_t CanvasWrapper::vprintf(bool newline, const char* format, va_list args) {
    return mCanvas.vprintf(newline, format, args);   
}

} // namespace Ui
} // namespace Bt

