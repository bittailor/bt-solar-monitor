//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::GetCommand
//
//*************************************************************************************************

#include "Bt/Sensors/VeDirect/GetCommand.h"

#include "Bt/Core/Log.h"
#include "Bt/Sensors/VeDirect/CommandExecutor.h"
#include "Bt/Sensors/VeDirect/Utility.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {

GetCommand::GetCommand(Register pRegister) : mCommandString{0} {
   mCommandString[0] = ':';
   mCommandString[1] = '7';
   uint8_t low = static_cast<uint16_t>(pRegister) & 0x00FF;
   uint8_t high = (static_cast<uint16_t>(pRegister) & 0xFF00) >> 8;
   byteToHex(low, mCommandString+2);
   byteToHex(high, mCommandString+4);
   mCommandString[6] = '0';
   mCommandString[7] = '0';
   mCommandString[8] = 0;
   uint8_t check = checksum(mCommandString);
   byteToHex(check, mCommandString+8);
}

GetCommand::~GetCommand() {

}

std::pair<bool,int16_t> GetCommand::execute(Stream& pStream) {
   const char* response = CommandExecutor(pStream, mCommandString).execute();
   if(response != nullptr && strlen(response) >= 11) {
      char number[5] = {0};
      number[0] = response[10];
      number[1] = response[11];
      number[2] = response[8];
      number[3] = response[9];
      BT_CORE_LOG_DEBUG("hex>%s", number);
      int value = strtol(number, nullptr, 16);
      BT_CORE_LOG_DEBUG("value>%d", value);
      return std::pair<bool,int16_t>(true,value);
   }
   return std::pair<bool,int16_t>(false,0);
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
