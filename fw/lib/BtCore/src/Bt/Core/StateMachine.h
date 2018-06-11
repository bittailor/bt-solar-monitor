//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StateMachine
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StateMachine__h
#define INC__Bt_Core_StateMachine__h

#include <stdint.h>
#include <deque>
#include <functional>
#include "Bt/Core/I_Time.h"
#include "Bt/Core/I_Runnable.h"
#include "Bt/Core/Scheduling.h"
#include "Bt/Core/Singleton.h"
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

      StateMachine() : mProcessingEvent(false), mScheduling(Scheduling::never()), mTimerActive(false), mStartTime(0), mInterval(0) {
      }

      ~StateMachine() {

      }

      virtual Scheduling workcycle() {
         if(!mTimerActive) {
            return mScheduling;
         }

         uint32_t diff = Singleton<I_Time>::instance()->milliseconds() - mStartTime;
         if (diff >= mInterval) {
            mTimerActive = false;
            mInterval = 0;
            mStartTime = 0;
            mCurrentState->timeUp();
            return Scheduling::immediately();
         }

         Scheduling timerScheduling = Scheduling::inMillis(mInterval - diff);

         return std::min(mScheduling, timerScheduling);
      }

   protected:

      void handle(void (State::*pEvent)()) {
         if (mProcessingEvent) {
            mQueue.push_back(pEvent);
            return;
         }
         mProcessingEvent = true;
         (mCurrentState->*pEvent)();
         processPending();
         mProcessingEvent = false;
      }

      template<typename Arg>
      void handle(void (State::*pEvent)(Arg),Arg pArg) {
         if (mProcessingEvent) {
            mQueue.push_back(std::bind(pEvent,std::placeholders::_1,pArg));
            return;
         }
         mProcessingEvent = true;
         (mCurrentState->*pEvent)(pArg);
         processPending();
         mProcessingEvent = false;
      }

      template<typename Arg1, typename Arg2>
      void handle(void (State::*pEvent)(Arg1,Arg2), Arg1 pArg1, Arg2 pArg2) {
         if (mProcessingEvent) {
            mQueue.push_back(std::bind(pEvent,std::placeholders::_1, pArg1, pArg2));
            return;
         }
         mProcessingEvent = true;
         (mCurrentState->*pEvent)(pArg1, pArg2);
         processPending();
         mProcessingEvent = false;
      }

      template<typename Arg1, typename Arg2, typename Arg3>
      void handle(void (State::*pEvent)(Arg1,Arg2,Arg3), Arg1 pArg1, Arg2 pArg2, Arg3 pArg3) {
         if (mProcessingEvent) {
            mQueue.push_back(std::bind(pEvent,std::placeholders::_1, pArg1, pArg2, pArg3));
            return;
         }
         mProcessingEvent = true;
         (mCurrentState->*pEvent)(pArg1, pArg2, pArg3);
         processPending();
         mProcessingEvent = false;
      }

      void processPending() {
         BT_CORE_LOG_INFO("%s: processPending %d", name() ,forPrintf(mQueue.size()));
         while(!mQueue.empty()) {
            mQueue.front()(mCurrentState);
            mQueue.pop_front();
         }
      }

      void nextState(StateBase& pState) {
         BT_CORE_LOG_INFO("%s: %s => %s", name(), mCurrentState->name(), pState.name());
         mCurrentState->onExit();
         mCurrentState = &pState;
         mCurrentState->onEnter();
      }

      void setTimer(uint32_t pInterval) {
         mTimerActive = true;
         mStartTime = Singleton<I_Time>::instance()->milliseconds();
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

      void scheduling(Scheduling pScheduling) {
         mScheduling = pScheduling;
      }

      StateBase& currentState() {
         return *mCurrentState;
      }

      virtual const char* name() =0;

   private:
      StateBase* mCurrentState;
      std::deque<std::function<void(State*)> > mQueue;
      bool mProcessingEvent;
      Scheduling mScheduling;
      bool mTimerActive;
      uint32_t mStartTime;
      uint32_t mInterval;

};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StateMachine__h
