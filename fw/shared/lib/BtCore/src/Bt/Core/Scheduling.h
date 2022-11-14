//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Scheduling
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Scheduling__h
#define INC__Bt_Core_Scheduling__h

#include <stdint.h>

namespace Bt {
namespace Core {

class Scheduling {
   public:
      enum Type {IMMEDIATELY, MILLIS_DELAY, SECONDS_DELAY, NEVER};

      Type type() { return mType; }
      uint32_t delay() { return mDelay; }
      const char* typeString();

      static Scheduling immediately() {
         return Scheduling(IMMEDIATELY, 0);
      }

      static Scheduling inMillis(uint32_t pDelay) {
         return Scheduling(MILLIS_DELAY, pDelay);
      }

      static Scheduling inSeconds(uint32_t pDelay) {
         return Scheduling(SECONDS_DELAY, pDelay);
      }

      static Scheduling never() {
         return Scheduling(NEVER, 0);
      }

      friend bool operator<(const Scheduling& l, const Scheduling& r)
      {
         if(l.mType == r.mType) {
            return l.mDelay < r.mDelay;
         }
         return  l.mType < r.mType;
      }

   private:
      Scheduling(Type pType, uint32_t pDelay)
      :mType(pType), mDelay(pDelay) {
      }

      Type mType;
      uint32_t mDelay;
};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Scheduling__h
