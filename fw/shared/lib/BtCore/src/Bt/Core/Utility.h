//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Utility
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Utility__h
#define INC__Bt_Core_Utility__h

#include <stddef.h>

namespace Bt {
namespace Core {

template<typename T, size_t N> constexpr size_t sizeOfArry(const T (&array)[N]) {
   return N;
}

template<typename T, typename Iterator>
void binaryAppend(const T& value, Iterator iterator){
   size_t size = sizeof(T);
   for(int i = size - 1 ; i >= 0 ; --i) {
      *iterator = (value >> (i*8)) & 0xff;
      iterator++;
   }
}

template<typename T, typename Iterator>
T binaryRead(Iterator iterator){
   T t = 0;
   size_t size = sizeof(T);
   for(int i = 0 ; i < size ; ++i) {
      t = (t << 8) | (*iterator & 0xff);
      iterator++;
   }
   return t;
}

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Utility__h
