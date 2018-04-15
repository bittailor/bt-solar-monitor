//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PlacementPointer
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PlacementPointer__h
#define INC__Bt_Core_PlacementPointer__h

#include <utility>
#include <stdint.h>

namespace Bt {
namespace Core {

template<typename T>
class PlacementPointer {
   public:

      PlacementPointer():mDisposed(true){
      }

      T& operator*() {
         return *typed();
      }

      T* operator-> ()
      {
         return typed();
      }

      void create() {
         new (raw()) T();
         mDisposed = false;
      }

      template<typename... Args>
      void create(Args&&... pArgs) {
         new (raw()) T(std::forward<Args>(pArgs)...);
         mDisposed = false;
      }

      void dispose() {
         typed()->~T();
         mDisposed = true;
      }

      explicit operator bool() const {
         return !mDisposed;
      }

   private:
      // Constructor to prohibit copy construction.
      PlacementPointer(const PlacementPointer&);

      // Operator= to prohibit copy assignment
      PlacementPointer& operator=(const PlacementPointer&);

      T* typed() {
         return reinterpret_cast<T*>(mMemory);
      }

      void* raw() {
         return mMemory;
      }

      bool mDisposed;
      uint8_t mMemory[sizeof(T)];
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PlacementPointer__h
