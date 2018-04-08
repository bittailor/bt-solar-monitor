//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Cloud
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Cloud__h
#define INC__Bt_Net_Cloud__h

#include <deque>
#include <vector>
#include <functional>
#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>
#include <Bt/Core/StateMachine.h>

namespace Bt {
namespace Net {

template<typename TRadio, typename TClient>
class CloudState {
   public:
      virtual ~CloudState() {}
      virtual void executeConnected(std::function<void (TClient&)> pExecutor){};
      virtual void OnSystemEvent(system_event_t event, int param){}
};

class CloudBase {
   public:

      virtual ~CloudBase();
      virtual void begin();

   protected:
      virtual void OnSystemEvent(system_event_t event, int param) =0;
   private:
      friend void handleSystemEventForCloud(system_event_t event, int param, void*);
      static std::vector<CloudBase*> sClouds;
};

template<typename TRadio, typename TClient>
class Cloud
         : public Core::StateMachine<CloudState<TRadio,TClient>, Cloud<TRadio,TClient> >
         , private CloudBase
{
   public:
      typedef Core::StateMachine<CloudState<TRadio,TClient>, Cloud<TRadio,TClient> > Parent;
      typedef TClient Client;

      Cloud(TRadio& pRadio, TClient& pCloud, const char* pEventNameStatus)
      : mRadio(pRadio)
      , mCloud(pCloud)
      , mEventNameStatus(pEventNameStatus)
      , mInitial(*this)
      , mRadioOff(*this)
      , mRadioConnecting(*this)
      , mCloudConnecting(*this)
      , mCloudConnected(*this)
      , mCloudDisconnecting(*this)
      , mRadioDisconnecting(*this) {
      }

      Cloud(const Cloud&) = delete;
      Cloud& operator=(const Cloud&) = delete;

      ~Cloud() {
      }

      virtual void begin() {
         CloudBase::begin();
         this->init(mInitial);
      }

      void executeConnected(std::function<void (TClient&)> pExecutor) {
         this->handle(&CloudState<TRadio,TClient>::executeConnected, pExecutor);
      }

      virtual void OnSystemEvent(system_event_t event, int param) {
         this->handle(&CloudState<TRadio,TClient>::OnSystemEvent, event, param);
      }

//      void executeConnected(std::function<void (Client&)> pExecutor) {
//         unsigned long timer = millis();
//         BT_CORE_LOG_INFO("mRadio.on() ...");
//         mRadio.on();
//         BT_CORE_LOG_INFO("... mRadio.on() DONE");
//         BT_CORE_LOG_INFO("mRadio.connect() ...");
//         mRadio.connect();
//         BT_CORE_LOG_INFO("... mRadio.connect() DONE");
//         while(! mRadio.ready() &&  mRadio.connecting()) {
//            BT_CORE_LOG_INFO("Check - mRadio.ready()");
//            mCloud.process();
//         }
//         mCloud.process();
//
//         BT_CORE_LOG_INFO("mRadio.connect() ...");
//         mCloud.connect();
//         BT_CORE_LOG_INFO("... mRadio.connect() DONE");
//
//         BT_CORE_LOG_INFO("while(!mCloud.connected() ...");
//         while(!mCloud.connected()) {
//            mCloud.process();
//         }
//         BT_CORE_LOG_INFO("... while(!mCloud.connected() DONE");
//
//         mCloud.publish(mEventNameStatus, "online", WITH_ACK);
//
//         pExecutor(mCloud);
//         mCloud.process();
//
//         mCloud.publish(mEventNameStatus, "offline", WITH_ACK);
//
//         BT_CORE_LOG_INFO("mRadio.disconnect() ...");
//         mCloud.disconnect();
//         BT_CORE_LOG_INFO("... mRadio.disconnect() DONE");
//
//         BT_CORE_LOG_INFO("while(mCloud.connected() ...");
//         while(mCloud.connected()) {
//            mCloud.process();
//         }
//         BT_CORE_LOG_INFO("... while(mCloud.connected() DONE");
//
//         {
//            unsigned long start = millis();
//            while(millis() - start <  1000) {
//               mCloud.process();;
//            }
//         }
//
//         BT_CORE_LOG_INFO("mRadio.disconnect() ...");
//         mRadio.disconnect();
//         BT_CORE_LOG_INFO("... mRadio.disconnect() DONE");
//         BT_CORE_LOG_INFO("mRadio.off() ...");
//         mRadio.off();
//         BT_CORE_LOG_INFO("... mRadio.off() DONE");
//         timer = millis() - timer;
//
//         BT_CORE_LOG_INFO("executeConnected took %lu ms",timer);
//         BT_CORE_LOG_DEBUG("free memory: %lu", System.freeMemory());
//      }

      virtual const char* name() {
         return "Cloud";
      }

   private:

      class Initial : public Parent::StateBase  {
         public:
         Initial(Cloud& pController):Parent::StateBase(pController){}

         virtual const char* name() {
            return "Initial";
         }

         virtual void onEnter() {
            this->mController->mRadio.off();
            this->mController->nextState(this->mController->mRadioOff);
         }
      };

      class RadioOff : public Parent::StateBase  {
         public:
            RadioOff(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "RadioOff";
            }

            virtual void onEnter() {
               this->mController->scheduling(Core::Scheduling::never());
            }

            virtual void onExit() {
               this->mController->scheduling(Core::Scheduling::immediately());
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               this->mController->mExecutors.push_back(pExecutor);
               BT_CORE_LOG_INFO("mRadio.on ...");
               this->mController->mRadio.on();
               BT_CORE_LOG_INFO("... mRadio.on");
               BT_CORE_LOG_INFO("mRadio.connect ...");
               this->mController->mRadio.connect();
               BT_CORE_LOG_INFO("... mRadio.connect");
               this->mController->nextState(this->mController->mRadioConnecting);
            };
      };

      class RadioConnecting : public Parent::StateBase  {
         public:
            RadioConnecting(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "RadioConnecting";
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               this->mController->mExecutors.push_back(pExecutor);
            }

            virtual void OnSystemEvent(system_event_t event, int param) {
               if(event == network_status && param == network_status_connected) {
                  this->mController->mCloud.connect();
                  this->mController->nextState(this->mController->mCloudConnecting);
               }
            }
      };

      class CloudConnecting : public Parent::StateBase  {
         public:
            CloudConnecting(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "CloudConnecting";
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               this->mController->mExecutors.push_back(pExecutor);
            }

            virtual void OnSystemEvent(system_event_t event, int param) {
               if(event == cloud_status && param == cloud_status_connected) {
                  this->mController->nextState(this->mController->mCloudConnected);
               }
            }
      };

      class CloudConnected : public Parent::StateBase  {
         public:
            CloudConnected(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "CloudConnected";
            }

            virtual void onEnter() {
               while(!this->mController->mExecutors.empty()) {
                  auto executor = this->mController->mExecutors.front();
                  this->mController->mExecutors.pop_front();
                  executor(this->mController->mCloud);
               }
               this->mController->mCloud.disconnect();
               this->mController->nextState(this->mController->mCloudDisconnecting);
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               pExecutor(this->mController->mCloud);
            }

      };

      class CloudDisconnecting : public Parent::StateBase  {
         public:
            CloudDisconnecting(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "CloudDisconnecting";
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               this->mController->mExecutors.push_back(pExecutor);
               this->mController->mCloud.connect();
               this->mController->nextState(this->mController->mCloudConnecting);
            }

            virtual void OnSystemEvent(system_event_t event, int param) {
               if(event == cloud_status && param == cloud_status_disconnected) {
                  this->mController->mRadio.disconnect();
                  this->mController->nextState(this->mController->mRadioDisconnecting);
               }
            }
      };

      class RadioDisconnecting : public Parent::StateBase  {
         public:
            RadioDisconnecting(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "RadioDisconnecting";
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               this->mController->mExecutors.push_back(pExecutor);
               this->mController->mRadio.connect();
               this->mController->nextState(this->mController->mRadioConnecting);
            }

            virtual void OnSystemEvent(system_event_t event, int param) {
               if(event == network_status && param == network_status_disconnected) {
                  this->mController->mRadio.off();
                  this->mController->nextState(this->mController->mRadioOff);
               }
            }
      };

      TRadio& mRadio;
      TClient& mCloud;
      std::deque<std::function<void (Client&)> > mExecutors;
      const char* mEventNameStatus;

      Initial mInitial;
      RadioOff mRadioOff;
      RadioConnecting mRadioConnecting;
      CloudConnecting mCloudConnecting;
      CloudConnected mCloudConnected;
      CloudDisconnecting mCloudDisconnecting;
      RadioDisconnecting mRadioDisconnecting;
      
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Cloud__h
