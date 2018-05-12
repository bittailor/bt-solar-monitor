//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StrCmp
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StrCmp__h
#define INC__Bt_Core_StrCmp__h

#include <string.h>

namespace Bt {
namespace Core {

struct StrCmp
{
   bool operator()(char const *a, char const *b)
   {
      return strcmp(a, b) < 0;
   }
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StrCmp__h
