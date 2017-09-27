//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::PublishFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_PublishFilter__h
#define INC__Bt_SolarMonitor_PublishFilter__h

#include <stddef.h>


#include <Bt/Core/Log.h>

namespace Bt {
namespace SolarMonitor {

template<typename Radio, typename Cloud>
class PublishFilter
{
   public:
      typedef const char* MessageString;

      PublishFilter(Radio& pRadio, Cloud& pCloud): mRadio(pRadio), mCloud(pCloud) {
      }
      PublishFilter(const PublishFilter&) = delete;
      PublishFilter& operator=(const PublishFilter&) = delete;

      void consume(const MessageString* pMessages, size_t pSize) {
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

         for (size_t msgCounter = 0; msgCounter < pSize; ++msgCounter) {
            bool ack = mCloud.publish("e/2/data", pMessages[msgCounter], WITH_ACK);
            BT_CORE_LOG_INFO(" ... mCloud.publish(%d) %d", strlen(pMessages[msgCounter]), ack);
            mCloud.process();
         }

         {
            unsigned long start = millis();
            while(millis() - start <  10000) {
               mCloud.process();;
            }
         }

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

         BT_CORE_LOG_INFO("publish took %lu ms",timer);
         BT_CORE_LOG_INFO("free memory: %lu", System.freeMemory());
      }

   private:
      Radio& mRadio;
      Cloud& mCloud;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_PublishFilter__h
