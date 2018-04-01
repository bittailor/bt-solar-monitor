//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StateMachine
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StateMachine__h
#define INC__Bt_Core_StateMachine__h

#include <stdint.h>
#include "Bt/Core/I_Time.h"
#include "Bt/Core/I_Runnable.h"
#include "Bt/Core/Log.h"

namespace Bt {
namespace Core {

template<typename State, typename Implementation>
class StateMachine : public Bt::Core::I_Runnable
{
   public:

      class StateBase : public State {
         public:
            StateBase(Implementation& pImplementation):mController(&pImplementation){}
            virtual void onEnter(){}
            virtual void onExit(){}
            virtual void timeUp(){}
            virtual const char* name() = 0;

         protected:
            Implementation* mController;
      };

      StateMachine(I_Time& pTime):mTime(&pTime), mTimerActive(false), mStartTime(0), mInterval(0) {
      }

      ~StateMachine() {

      }

      virtual Scheduling workcycle() {
         if(!mTimerActive) {
            return Scheduling::never();
         }

         uint32_t diff = mTime->milliseconds() - mStartTime;
         if (diff >= mInterval) {
            mTimerActive = false;
            mInterval = 0;
            mStartTime = 0;
            mCurrentState->timeUp();
            return Scheduling::immediately();
         }

         return Scheduling::inMillis(mInterval - diff);
      }

   protected:

      void handle(void (State::*pEvent)()) {
         (mCurrentState->*pEvent)();
      }

      template<typename R>
      R handle(R (State::*pEvent)()) {
         return (mCurrentState->*pEvent)();
      }

      template<typename Arg>
      void handle(void (State::*pEvent)(Arg),Arg pArg) {
         (mCurrentState->*pEvent)(pArg);
      }

      template<typename Arg1, typename Arg2>
      void handle(void (State::*pEvent)(Arg1,Arg2), Arg1 pArg1, Arg2 pArg2) {
         (mCurrentState->*pEvent)(pArg1, pArg2);
      }

      template<typename R, typename Arg1, typename Arg2>
      R handle(R (State::*pEvent)(Arg1,Arg2), Arg1 pArg1, Arg2 pArg2) {
         return (mCurrentState->*pEvent)(pArg1, pArg2);
      }

      template<typename R, typename Arg1, typename Arg2, typename Arg3>
      R handle(R (State::*pEvent)(Arg1,Arg2,Arg3), Arg1 pArg1, Arg2 pArg2, Arg3 pArg3) {
         return (mCurrentState->*pEvent)(pArg1, pArg2, pArg3);
      }


      void nextState(StateBase& pState) {
         BT_CORE_LOG_DEBUG("%s: %s => %s", name(), mCurrentState->name(), pState.name());
         mCurrentState->onExit();
         mCurrentState = &pState;
         mCurrentState->onEnter();
      }

      void setTimer(uint32_t pInterval) {
         mTimerActive = true;
         mStartTime = mTime->milliseconds();
         mInterval = pInterval;
      }

      void resetTimer() {
         mTimerActive = false;
         mStartTime = 0;
         mInterval = 0;
      }

      void init(StateBase& pInitialState) {
         mCurrentState = &pInitialState;
         mCurrentState->onEnter();
      }

      bool checkState(StateBase& pState) {
         return mCurrentState ==  &pState;
      }

      StateBase& currentState() {
         return *mCurrentState;
      }

      virtual const char* name() =0;

   private:
      I_Time* mTime;
      StateBase* mCurrentState;
      bool mTimerActive;
      uint32_t mStartTime;
      uint32_t mInterval;

};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StateMachine__h
