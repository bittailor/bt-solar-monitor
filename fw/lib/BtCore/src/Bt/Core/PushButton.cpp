//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PushButton
//
//*************************************************************************************************

#include "Bt/Core/PushButton.h"

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

PushButton::PushButton(Core::I_Time& pTime, I_DigitalIn& pInput, I_Listener& pListener)
: StateMachine<PushButtonState,PushButton>(pTime)
, mInput(&pInput)
, mListener(&pListener)
, mInitial(*this)
, mReleased(*this)
, mToReleased(*this)
, mPressed(*this)
, mToPressed(*this) {

}

//-------------------------------------------------------------------------------------------------

PushButton::~PushButton() {

}

//-------------------------------------------------------------------------------------------------

const char* PushButton::name() {
   return "PB";
}

//-------------------------------------------------------------------------------------------------

void PushButton::begin() {
   init(mInitial);
}

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
