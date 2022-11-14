//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PushButton
//
//*************************************************************************************************

#include "Bt/Core/PushButton.h"

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

PushButton::PushButton(I_DigitalIn& pInput, I_Listener& pListener)
: StateMachine<PushButtonState,PushButton>()
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
