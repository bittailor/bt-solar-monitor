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

namespace Bt {
namespace SolarMonitor {

template<typename TCloud>
class PublishFilter
{
   public:
      typedef const char* MessageString;

      PublishFilter(TCloud& pCloud, const char* pEventName): mCloud(pCloud), mEventName(pEventName) {
      }
      PublishFilter(const PublishFilter&) = delete;
      PublishFilter& operator=(const PublishFilter&) = delete;

      void consume(const MessageString* pMessages, size_t pSize) {
         mCloud.executeConnected([&] (typename TCloud::Client& client) {
            int republishLimit = 10;
            for (size_t msgCounter = 0; msgCounter < pSize; ++msgCounter) {
               bool ack = client.publish(mEventName, pMessages[msgCounter], WITH_ACK);
               BT_CORE_LOG_INFO("cloud.publish(%d) => %d", strlen(pMessages[msgCounter]), ack);
               client.process();
               if(!ack) {
                  republishLimit--;
                  if(republishLimit > 0) {
                     BT_CORE_LOG_WARN("publish failed => sleep and try to re-publish");
                     client.process();
                     Bt::Core::msSleep(1000);
                     client.process();
                     msgCounter--;
                  } else {
                     BT_CORE_LOG_WARN("republish limit reached => skip all!");
                     return;
                  }
               } else {
                  republishLimit = 10;
               }
            }
         });
      }

   private:
      TCloud& mCloud;
      const char* mEventName;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_PublishFilter__h
