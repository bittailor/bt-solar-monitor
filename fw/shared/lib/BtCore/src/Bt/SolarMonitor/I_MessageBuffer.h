//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_MessageBuffer
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_MessageBuffer__h
#define INC__Bt_SolarMonitor_I_MessageBuffer__h

#include <stddef.h>

namespace Bt {
namespace SolarMonitor {

class I_MessageBuffer {
   public:
      virtual ~I_MessageBuffer() {}
      virtual const char* peak() const =0;
      virtual void pop()=0;
      virtual bool empty() const =0;
      virtual size_t count() const =0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_MessageBuffer__h
