//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::PublishFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_PublishFilter__h
#define INC__Bt_SolarMonitor_PublishFilter__h

#include <stddef.h>

#include <Bt/Core/Log.h>
#include <Bt/Core/Sleep.h>
#include "Bt/SolarMonitor/I_MessageBuffer.h"

namespace Bt {
namespace SolarMonitor {

template<typename TCloud>
class PublishFilter
{
   public:
      typedef const char* MessageString;

      PublishFilter(TCloud& pCloud, const char* pEventName, I_MessageBuffer& pMessageBuffer)
      : mCloud(pCloud), mEventName(pEventName), mMessageBuffer(pMessageBuffer), mRepublishLimit(10) {
      }
      PublishFilter(const PublishFilter&) = delete;
      PublishFilter& operator=(const PublishFilter&) = delete;

      void publish() {
         BT_CORE_LOG_INFO("PublishFilter A publish %u ", forPrintf(mMessageBuffer.count()));
         mCloud.executeConnected([this] (typename TCloud::Client& client) {
            BT_CORE_LOG_INFO("PublishFilter B publish %u ", forPrintf(mMessageBuffer.count()));
            while(!mMessageBuffer.empty()) {
               const char* message = mMessageBuffer.peak();
               BT_CORE_LOG_INFO("b cloud.publish(%d)", strlen(message));
               bool ack = client.publish(mEventName, message, WITH_ACK);
               BT_CORE_LOG_INFO("e cloud.publish(%d) => %d", strlen(message), ack);
               client.process();
               if(!ack) {
                  mRepublishLimit--;
                  if(mRepublishLimit > 0) {
                     BT_CORE_LOG_WARN("publish failed => sleep and try to re-publish");
                     return false;
                  } else {
                     BT_CORE_LOG_WARN("republish limit reached => skip all!");
                     mRepublishLimit = 10;
                     return true;
                  }
               } else {
                  mRepublishLimit = 10;
                  mMessageBuffer.pop();
               }
            }
            return true;
         });
      }

   private:
      TCloud& mCloud;
      const char* mEventName;
      I_MessageBuffer& mMessageBuffer;
      int mRepublishLimit;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_PublishFilter__h
