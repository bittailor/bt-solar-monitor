//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_PushButton
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_PushButton__h
#define INC__Bt_Core_I_PushButton__h

namespace Bt {
namespace Core {

class I_PushButton {
   public:
      class I_Listener {
         public:
            virtual ~I_Listener() {}
            virtual void pressed()=0;
            virtual void released()=0;
      };



      virtual ~I_PushButton() {}

      virtual void change()=0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_PushButton__h
