//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Ui::CanvasWrapper
//
//*************************************************************************************************

#include "Bt/Ui/CanvasWrapper.h"

#include <stdarg.h>

namespace Bt {
namespace Ui {

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

