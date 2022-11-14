//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::IntrusiveList
//
//*************************************************************************************************

#include <Bt/Core/IntrusiveList.h>

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

IntrusiveListBase::IntrusiveListBase()
:mHead(0), mTail(0) {

}

//-------------------------------------------------------------------------------------------------
IntrusiveListBase::~IntrusiveListBase() {

}

//-------------------------------------------------------------------------------------------------

void IntrusiveListBase::pushBack(ElementBase* iElement) {
   if (mTail == 0) {
      mHead = iElement;
      mTail = iElement;
      iElement->reset();
   } else {
      iElement->mPrevious = mTail;
      iElement->mNext = 0;
      mTail->mNext = iElement;
      mTail = iElement;
   }
}

//-------------------------------------------------------------------------------------------------

void IntrusiveListBase::remove(ElementBase* iElement) {
   if (mHead == iElement && mTail == iElement) {
      mHead = 0;
      mTail = 0;
      iElement->reset();
      return;
   }

   if (mHead == iElement) {
      mHead = iElement->mNext;
      mHead->mPrevious = 0;
      iElement->reset();
      return;
   }

   if (mTail == iElement) {
      mTail = iElement->mPrevious;
      mTail->mNext = 0;
      iElement->reset();
      return;
   }

   iElement->mPrevious->mNext = iElement->mNext;
   iElement->mNext->mPrevious = iElement->mPrevious;

}

//-------------------------------------------------------------------------------------------------


} // namespace Core
} // namespace Bt
