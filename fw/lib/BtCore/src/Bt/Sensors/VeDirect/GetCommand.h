//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::GetCommand
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_GetCommand__h
#define INC__Bt_Sensors_VeDirect_GetCommand__h

#include <utility>
#include <stdint.h>

#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>

#include "Bt/Sensors/VeDirect/CommandExecutor.h"
#include "Bt/Sensors/VeDirect/Utility.h"



namespace Bt {
namespace Sensors {
namespace VeDirect {

enum OldRegisters : uint16_t {
   ChargerCurrent = 0xEDD7,
   ChargerVoltage = 0xEDD5,

   PanelPower = 0xEDBC,
   PanelVoltage = 0xEDBB,

   LoadCurrent = 0xEDAD,
   LoadOutputState = 0xEDA8,

   BatteryMaximumCurrent = 0xEDF0,

};

template<typename T>
class GetCommand
{
   public:
      GetCommand(uint16_t pRegister);
      std::pair<bool,T> execute(Stream& pStream);

   private:
      char mCommandString[11];
      

};

template<>
class GetCommand<bool>
{
   public:
      GetCommand(uint16_t pRegister) : mCmd(pRegister) {
      }

      std::pair<bool,bool> execute(Stream& pStream) {
         std::pair<bool,uint8_t> value = mCmd.execute(pStream);
         return std::pair<bool,bool>(value.first, value.second > 0);
      }

   private:
      GetCommand<uint8_t> mCmd;
};


//-------------------------------------------------------------------------------------------------

template<typename T>
GetCommand<T>::GetCommand(uint16_t pRegister) : mCommandString{0} {
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

template<typename T>
std::pair<bool,T> GetCommand<T>::execute(Stream& pStream) {
   const char* response = CommandExecutor(pStream, mCommandString).execute();
   if(response == nullptr) {
      BT_CORE_LOG_WARN("command executor failed");
      return std::pair<bool,int16_t>(false,0);
   }
   size_t responseLenght =  strlen(response);
   size_t valueCharactersLenght = responseLenght - strlen(mCommandString);
   if(sizeof(T) * 2 != valueCharactersLenght) {
      BT_CORE_LOG_WARN("wrong valueCharactersLenght %zd * 2 != %zd", sizeof(T), valueCharactersLenght);
      return std::pair<bool,int16_t>(false,0);
   }
   size_t indexValueStart = responseLenght - 2 - valueCharactersLenght;
   T value = T();
   for(int i = valueCharactersLenght - 2 ; i >= 0 ; i -= 2) {
      uint8_t byte = hexToByte(response + indexValueStart + i);
      value = (value << 8) + byte;
   }
   return std::pair<bool,T>(true,value);
}




} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_GetCommand__h
