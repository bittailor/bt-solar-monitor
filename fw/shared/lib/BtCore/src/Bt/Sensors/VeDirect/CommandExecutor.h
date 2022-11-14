//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::CommandExecutor
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_CommandExecutor__h
#define INC__Bt_Sensors_VeDirect_CommandExecutor__h

#include <Bt/Core/Platform.h>
#include <Bt/Core/Timer.h>

namespace Bt {
namespace Sensors {
namespace VeDirect {

class CommandExecutor
{
   public:
      CommandExecutor(Stream& pStream, const char* pCommand);
      CommandExecutor(const CommandExecutor&) = delete;
      CommandExecutor& operator=(const CommandExecutor&) = delete;
      ~CommandExecutor();

      const char* execute();

   private:
      static const size_t BUFFER_SIZE = 100;
      enum State {SUCCESS,CONTINUE,FAIL};
      typedef State (CommandExecutor::*StateFunction)();

      void flush();
      State waitForStartCharacter();
      State readUntilEndCharacter();
      State checkMatch();
      State checkCheckSum();

      Stream& mStream;
      const char* mCommand;
      char mBuffer[BUFFER_SIZE];
      size_t mBufferIndex;
      Bt::Core::Timer mTimer;
      StateFunction mStateFunction;
      



};

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_CommandExecutor__h
