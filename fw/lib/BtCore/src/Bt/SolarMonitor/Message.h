//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Message
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Message__h
#define INC__Bt_SolarMonitor_Message__h

#include <array>
#include <string.h>

namespace Bt {
namespace SolarMonitor {

class Message {
   public:
      static const size_t MESSAGE_BUFFER_LENGHT = 512;

      Message() {
         mBuffer[0] = 0;
      }

      Message(const char* pMessage) {
         strncpy(mBuffer, pMessage, MESSAGE_BUFFER_LENGHT);
      }

      Message(const Message& pOther) {
         strncpy(mBuffer, pOther.mBuffer, MESSAGE_BUFFER_LENGHT);
      }

      Message& operator=(const Message& pOther) {
         if(this != &pOther) {
            strncpy(mBuffer, pOther.mBuffer, MESSAGE_BUFFER_LENGHT);
         }
         return *this;
      }

      const char* message() const {
         return mBuffer;
      }

   private:
      char mBuffer[MESSAGE_BUFFER_LENGHT];
};


} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Message__h
