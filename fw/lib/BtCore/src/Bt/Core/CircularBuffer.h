//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::CircularBuffer
//
//*************************************************************************************************

#ifndef INC__Bt_Core_CircularBuffer__h
#define INC__Bt_Core_CircularBuffer__h

#include <array>
#include <Bt/Core/Log.h>

namespace Bt {
namespace Core {

template<typename T, size_t SIZE>
class CircularBuffer
{
   private:
      static const size_t INTERNAL_BUFFER_SIZE = SIZE;

   public:
      CircularBuffer()
      :mCount(0), mWriteIndex(0), mReadIndex(0){
      }

      CircularBuffer(const CircularBuffer&) = delete;

      CircularBuffer& operator=(const CircularBuffer&) = delete;

      ~CircularBuffer(){
      }

      void push(const T& pElement){
         mStorage[mWriteIndex] = pElement;
         mWriteIndex = (mWriteIndex + 1) % INTERNAL_BUFFER_SIZE;
         if(mCount == SIZE)
         {
            BT_CORE_LOG_WARN("CircularBuffer push overrides entry!");
            mReadIndex = (mReadIndex + 1) % INTERNAL_BUFFER_SIZE;
         }
         else {
            mCount++;
         }
         BT_CORE_LOG_DEBUG("CircularBuffer appended %d [%d|%d]", forPrintf(count()), forPrintf(mReadIndex), forPrintf(mWriteIndex)  );
      }

      virtual const T* peak() const {
         if(empty())
         {
            return nullptr;
         }
         return &mStorage[mReadIndex];
      }

      virtual void pop()
      {
         if(empty())
         {
            BT_CORE_LOG_WARN("CircularBuffer pop on empty buffer!");
            return;
         }
         mReadIndex = (mReadIndex + 1) % INTERNAL_BUFFER_SIZE;
         mCount--;
         BT_CORE_LOG_DEBUG("CircularBuffer removed %d [%d|%d]", forPrintf(count()), forPrintf(mReadIndex), forPrintf(mWriteIndex)  );
      }

      virtual bool empty() const
      {
         return mCount == 0;
      }

      virtual size_t count() const {
         return mCount;
      }


   private:
      std::array<T, INTERNAL_BUFFER_SIZE> mStorage;
      size_t mCount;
      size_t mWriteIndex;
      size_t mReadIndex;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_CircularBuffer__h
