//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::EmulatedReader
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_EmulatedReader__h
#define INC__Bt_SolarMonitor_EmulatedReader__h

#include "Bt/SolarMonitor/Reader.h"

namespace Bt {
namespace SolarMonitor {


class EmulatedReader
{
   public:
      static const int NUMBER_OF_VALUES = Reader::NUMBER_OF_VALUES;
      typedef Reader::Readings Readings;

      EmulatedReader(Stream& pStreamA, Stream& pStreamB);
      EmulatedReader(const EmulatedReader&) = delete;
      EmulatedReader& operator=(const EmulatedReader&) = delete;
      ~EmulatedReader();

      Readings read();

   private:
      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_EmulatedReader__h
