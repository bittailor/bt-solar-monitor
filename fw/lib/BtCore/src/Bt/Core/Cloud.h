//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Cloud
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Cloud__h
#define INC__Bt_Core_Cloud__h

#include <functional>
#include <Bt/Core/Log.h>

namespace Bt {
namespace Core {

template<typename TRadio, typename TClient>
class Cloud
{
   public:
      typedef TClient Client;

      Cloud(TRadio& pRadio, TClient& pCloud, const char* pEventNameStatus): mRadio(pRadio), mCloud(pCloud), mEventNameStatus(pEventNameStatus) {
      }
      Cloud(const Cloud&) = delete;
      Cloud& operator=(const Cloud&) = delete;

      void executeConnected(std::function<void (Client&)> pExecutor) {
         unsigned long timer = millis();
         BT_CORE_LOG_INFO("mRadio.on() ...");
         mRadio.on();
         BT_CORE_LOG_INFO("... mRadio.on() DONE");
         BT_CORE_LOG_INFO("mRadio.connect() ...");
         mRadio.connect();
         BT_CORE_LOG_INFO("... mRadio.connect() DONE");
         while(! mRadio.ready() &&  mRadio.connecting()) {
            BT_CORE_LOG_INFO("Check - mRadio.ready()");
            mCloud.process();
         }
         mCloud.process();

         BT_CORE_LOG_INFO("mRadio.connect() ...");
         mCloud.connect();
         BT_CORE_LOG_INFO("... mRadio.connect() DONE");

         BT_CORE_LOG_INFO("while(!mCloud.connected() ...");
         while(!mCloud.connected()) {
            mCloud.process();
         }
         BT_CORE_LOG_INFO("... while(!mCloud.connected() DONE");

         mCloud.publish(mEventNameStatus, "online", WITH_ACK);

         pExecutor(mCloud);
         mCloud.process();

         mCloud.publish(mEventNameStatus, "offline", WITH_ACK);

         BT_CORE_LOG_INFO("mRadio.disconnect() ...");
         mCloud.disconnect();
         BT_CORE_LOG_INFO("... mRadio.disconnect() DONE");

         BT_CORE_LOG_INFO("while(mCloud.connected() ...");
         while(mCloud.connected()) {
            mCloud.process();
         }
         BT_CORE_LOG_INFO("... while(mCloud.connected() DONE");

         {
            unsigned long start = millis();
            while(millis() - start <  1000) {
               mCloud.process();;
            }
         }

         BT_CORE_LOG_INFO("mRadio.disconnect() ...");
         mRadio.disconnect();
         BT_CORE_LOG_INFO("... mRadio.disconnect() DONE");
         BT_CORE_LOG_INFO("mRadio.off() ...");
         mRadio.off();
         BT_CORE_LOG_INFO("... mRadio.off() DONE");
         timer = millis() - timer;

         BT_CORE_LOG_INFO("executeConnected took %lu ms",timer);
         BT_CORE_LOG_DEBUG("free memory: %lu", System.freeMemory());
      }

   private:
      TRadio& mRadio;
      TClient& mCloud;
      const char* mEventNameStatus;

      
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Cloud__h
