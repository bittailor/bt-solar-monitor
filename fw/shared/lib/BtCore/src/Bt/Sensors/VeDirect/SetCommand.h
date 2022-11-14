//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::SetCommand
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_SetCommand__h
#define INC__Bt_Sensors_VeDirect_SetCommand__h

#include <utility>
#include <stdint.h>

#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>

#include "Bt/Sensors/VeDirect/CommandExecutor.h"
#include "Bt/Sensors/VeDirect/Utility.h"



namespace Bt {
namespace Sensors {
namespace VeDirect {

template<typename T>
class SetCommand
{
   public:
      SetCommand(uint16_t pRegister);
      bool execute(Stream& pStream, T value);

   private:
      static const int BUFFER_SIZE = 30;
      char mCommandString[BUFFER_SIZE];

};

template<>
class SetCommand<bool>
{
   public:
      SetCommand(uint16_t pRegister) : mCmd(pRegister) {
      }

      bool execute(Stream& pStream, bool value) {
         return mCmd.execute(pStream, value);
      }

   private:
      SetCommand<uint8_t> mCmd;
};


//-------------------------------------------------------------------------------------------------

template<typename T>
SetCommand<T>::SetCommand(uint16_t pRegister) : mCommandString{0} {
   mCommandString[0] = ':';
   mCommandString[1] = '8';
   uint8_t low = static_cast<uint16_t>(pRegister) & 0x00FF;
   uint8_t high = (static_cast<uint16_t>(pRegister) & 0xFF00) >> 8;
   byteToHex(low, mCommandString+2);
   byteToHex(high, mCommandString+4);
   mCommandString[6] = '0';
   mCommandString[7] = '0';
   mCommandString[8] = 0;
   //uint8_t check = checksum(mCommandString);
   //byteToHex(check, mCommandString+8);
}

template<typename T>
bool SetCommand<T>::execute(Stream& pStream, T value) {
   if(sizeof(T) * 2 >= BUFFER_SIZE - 10) {
      BT_CORE_LOG_WARN("command buffer to small  %u >= %u", forPrintf(sizeof(T)), BUFFER_SIZE - 10);
      return false;
   }
   mCommandString[8] = 0;
   strAppendLittleEndianHex(mCommandString, value);
   uint8_t check = checksum(mCommandString);
   byteToHex(check, mCommandString + strlen(mCommandString));

   CommandExecutor commandExecutor(pStream, mCommandString);
   const char* response = commandExecutor.execute();
   if(response == nullptr) {
      BT_CORE_LOG_WARN("command executor failed");
      return false;
   }
   if(strcmp(mCommandString, response) != 0) {
      BT_CORE_LOG_WARN("set failed cmd->'%s' != rsp->'%s'", mCommandString, response);
      return false;
   }
   return true;
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_SetCommand__h
