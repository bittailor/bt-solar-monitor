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
#include "Bt/SolarMonitor/I_MessageBuffer.h"

namespace Bt {
namespace SolarMonitor {

template<size_t SIZE>
class MessageBufferSink : public I_MessageBuffer
{
   private:
      static const size_t BUFFER_SIZE = SIZE + 1;

   public:
      static const size_t MESSAGE_BUFFER_LENGHT = 256;

      MessageBufferSink()
      : mWriteIndex(0), mReadIndex(0)
      {}
      MessageBufferSink(const MessageBufferSink&) = delete;
      MessageBufferSink& operator=(const MessageBufferSink&) = delete;
      ~MessageBufferSink(){

      }

      void consume(const char* pMessage){
         strncpy(mStorage[mWriteIndex], pMessage, MESSAGE_BUFFER_LENGHT);
         mWriteIndex = (mWriteIndex + 1) % BUFFER_SIZE;
         if(mWriteIndex == mReadIndex)
         {
            BT_CORE_LOG_WARN("MessageBufferSink override buffered unsent message!");
            mReadIndex = (mReadIndex + 1) % BUFFER_SIZE;
         }
         BT_CORE_LOG_INFO("MessageBufferSink appended %d [%d|%d]", forPrintf(count()), forPrintf(mReadIndex), forPrintf(mWriteIndex)  );
      }

      virtual const char* peak() const {
         if(empty())
         {
            return nullptr;
         }
         return mStorage[mReadIndex];
      }

      virtual void pop()
      {
         if(empty())
         {
            BT_CORE_LOG_WARN("MessageBufferSink pop on empty buffer!");
            return;
         }
         mReadIndex = (mReadIndex + 1) % BUFFER_SIZE;
         BT_CORE_LOG_INFO("MessageBufferSink removed %d [%d|%d]", forPrintf(count()), forPrintf(mReadIndex), forPrintf(mWriteIndex)  );
      }

      virtual bool empty() const
      {
         return mWriteIndex == mReadIndex;
      }

      virtual size_t count() const {
         if(mWriteIndex >= mReadIndex) {
            return mWriteIndex - mReadIndex;
         }
         return mWriteIndex + BUFFER_SIZE - mReadIndex;
      }


   private:
      std::array<char[MESSAGE_BUFFER_LENGHT], BUFFER_SIZE> mStorage;
      size_t mWriteIndex;
      size_t mReadIndex;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MessageBufferSink__h
