//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PushButton
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PushButton__h
#define INC__Bt_Core_PushButton__h

#include "Bt/Core/I_PushButton.h"
#include "Bt/Core/I_DigitalIn.h"
#include "Bt/Core/StateMachine.h"

namespace Bt {
namespace Core {

class PushButtonState {
   public:
      virtual ~PushButtonState() {}
      virtual void change(){}
};

class PushButton : public I_PushButton, public StateMachine<PushButtonState, PushButton>
{
   public:
      PushButton(Core::I_Time& pTime, I_DigitalIn& pInput, I_Listener& pListener);
      ~PushButton();

      void begin();

      virtual void change() {
         handle(&PushButtonState::change);
      }

   protected:
      virtual const char* name();

   private:

      class Initial : public StateBase  {
         public:
         Initial(PushButton& pController):StateBase(pController){}

         virtual const char* name() {
            return "Initial";
         }

         virtual void onEnter() {
            if(mController->mInput->read()) {
               mController->nextState(mController->mReleased);
            } else {
               mController->nextState(mController->mPressed);
            }
         }
      };

      class Released : public StateBase{
         public:
            Released(PushButton& pController) :StateBase(pController){}

            virtual const char* name() {
               return "Released";
            }

            virtual void change(){
               if(!mController->mInput->read()) {
                  mController->nextState(mController->mToPressed);
               }
            }

      };

      class ToReleased : public StateBase{
         public:
            ToReleased(PushButton& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToReleased";
            }

            virtual void change(){
               if(!mController->mInput->read()) {
                  mController->nextState(mController->mPressed);
               }
            }

            virtual void onEnter(){
               mController->setTimer(100);
            }

            virtual void timeUp(){
               mController->nextState(mController->mReleased);
               mController->mListener->released();
            }
      };

      class Pressed : public StateBase{
         public:
            Pressed(PushButton& pController) :StateBase(pController){}

            virtual const char* name() {
               return "Pressed";
            }

            virtual void change(){
               if(mController->mInput->read()) {
                  mController->nextState(mController->mToReleased);
               }
            }
      };

      class ToPressed : public StateBase{
         public:
            ToPressed(PushButton& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToPressed";
            }

            virtual void change(){
               if(mController->mInput->read()) {
                  mController->nextState(mController->mReleased);
               }
            }

            virtual void onEnter(){
               mController->setTimer(100);
            }
            virtual void timeUp(){
               mController->nextState(mController->mPressed);
               mController->mListener->pressed();
            }
      };

      // Constructor to prohibit copy construction.
      PushButton(const PushButton&);

      // Operator= to prohibit copy assignment
      PushButton& operator=(const PushButton&);

      I_DigitalIn* mInput;
      I_Listener* mListener;
      Initial mInitial;
      Released mReleased;
      ToReleased mToReleased;
      Pressed mPressed;
      ToPressed mToPressed;


};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PushButton__h
