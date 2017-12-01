//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::TestLogger
//
//*************************************************************************************************

#include "Bt/TestLogger.h"
#include <fstream>

namespace Bt {

std::ostream& log() {
   static std::ofstream sLog("TestLog.txt", std::ofstream::app);
   return sLog;
}

} // namespace Bt
