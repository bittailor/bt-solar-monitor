   //*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MessageBufferSink
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MessageBufferSink__h
#define INC__Bt_SolarMonitor_MessageBufferSink__h

#include <array>
#include <string.h>

#include <Bt/Core/Log.h>
#include <Bt/Core/CircularBuffer.h>
#include "Bt/SolarMonitor/I_MessageBuffer.h"
#include "Bt/SolarMonitor/Message.h"

namespace Bt {
namespace SolarMonitor {

template<size_t SIZE>
class MessageBufferSink : public I_MessageBuffer
{
   public:
      MessageBufferSink() = default;
      MessageBufferSink(const MessageBufferSink&) = delete;
      MessageBufferSink& operator=(const MessageBufferSink&) = delete;

      void consume(const char* pMessage){
         mCircularBuffer.push(pMessage);
         BT_CORE_LOG_INFO("MessageBufferSink: new message added [count=%u]", forPrintf(mCircularBuffer.count()));
      }

      virtual const char* peak() const {
         const Message* message = mCircularBuffer.peak();
         return message ? message->message() : nullptr ;
      }

      virtual void pop()
      {
         mCircularBuffer.pop();
      }

      virtual bool empty() const
      {
         return mCircularBuffer.empty();
      }

      virtual size_t count() const {
         return mCircularBuffer.count();
      }


   private:

      Core::CircularBuffer<Message, SIZE> mCircularBuffer;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MessageBufferSink__h
