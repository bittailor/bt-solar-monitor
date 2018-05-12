//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::InterruptPushButton
//
//*************************************************************************************************

#include "Bt/Core/InterruptPushButton.h"

#include <array>
#include <inttypes.h>
#include <Bt/Core/Log.h>
#include <Bt/Core/Platform.h>

namespace Bt {
namespace Core {
namespace {
   // DBG_BT const size_t MSG_SIZE = 30;
   // DBG_BT std::array<char[MSG_SIZE],10> sMessages;
   // DBG_BT size_t sMessagesIndex = 0;
   // DBG_BT bool mToggle = false;
}

InterruptPushButton::InterruptPushButton(uint16_t pPin, std::function<void()> pOnClick)
: mPin(pPin), mOnClick(pOnClick), mPressed(false), mFire(false) {
}

InterruptPushButton::~InterruptPushButton() {
}

void InterruptPushButton::begin(){
   pinMode(mPin, INPUT_PULLDOWN);
   attachInterrupt(mPin, &InterruptPushButton::interruptHandler, this, CHANGE);
}

Scheduling InterruptPushButton::workcycle() {
   bool fire = false;

   // DBG_BT std::array<char[MSG_SIZE+1],30> messages;
   // DBG_BT size_t messagesIndex = 0;
   // DBG_BT ATOMIC_BLOCK(){
   // DBG_BT    if(sMessagesIndex != 0) {
   // DBG_BT       for (size_t i = 0; i < sMessagesIndex; ++i) {
   // DBG_BT          memcpy(messages[i],sMessages[i], MSG_SIZE);
   // DBG_BT       }
   // DBG_BT       messagesIndex = sMessagesIndex;
   // DBG_BT       sMessagesIndex = 0;
   // DBG_BT    }
   // DBG_BT }
   // DBG_BT
   // DBG_BT for (size_t i = 0; i < messagesIndex; ++i) {
   // DBG_BT    BT_CORE_LOG_INFO("M[%d]: %s", forPrintf(i), messages[i]);
   // DBG_BT }

   ATOMIC_BLOCK(){
      if(mFire) {
         fire = mFire;
         mFire = false;
      }
   }
   if(fire) {
      BT_CORE_LOG_INFO("Pin %d pressed", mPin);
      if(mOnClick) {
         mOnClick();
      }
   }
   return Scheduling::never();
}

void InterruptPushButton::interruptHandler() {
   bool pressed = pinReadFast(mPin) == HIGH;

   if(pressed && !mPressed) {
      mTimer = Timer(50);
      // DBG_BT snprintf(sMessages[sMessagesIndex++], MSG_SIZE, "%d pressed  t=%" PRIu32, mPin, millis());
   } else if(!pressed && mPressed) {
      // DBG_BT snprintf(sMessages[sMessagesIndex++], MSG_SIZE, "%d !pressed e=%s t=%" PRIu32, mPin, mTimer.expired() ? "true": "false", millis());
      if(mTimer.expired()) {
         // DBG_BT digitalWrite(7,mToggle);
         // DBG_BT mToggle =! mToggle;
         mFire = true;
      }
   } else {
      // DBG_BT snprintf(sMessages[sMessagesIndex++], MSG_SIZE, "%d s=n s=%s n=%s  t=%" PRIu32, mPin, mPressed ? "true": "false", pressed ? "true": "false",  millis());
   }
   // DBG_BT if(sMessagesIndex >= sMessages.size()) {
   // DBG_BT    sMessagesIndex = 0;
   // DBG_BT    snprintf(sMessages[sMessagesIndex++], MSG_SIZE, "overflow");
   // DBG_BT }
   mPressed = pressed;
}

} // namespace Core
} // namespace Bt
