//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Cloud
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Cloud__h
#define INC__Bt_Net_Cloud__h

#include <inttypes.h>
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
      , mRadioDisconnecting(*this){
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
               this->mController->setTimer(10*60*1000);
            }

            virtual void executeConnected(std::function<void (TClient&)> pExecutor) {
               this->mController->connectStartTime = millis();
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

            virtual void timeUp(){
               BT_CORE_LOG_ERROR("Cloud connect timeout!");
               this->mController->mExecutors.clear();
               this->mController->mRadio.off();
               this->mController->nextState(this->mController->mRadioOff);
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

            virtual void timeUp(){
               BT_CORE_LOG_ERROR("Cloud connect timeout!");
               this->mController->mExecutors.clear();
               this->mController->mRadio.disconnect();
               this->mController->nextState(this->mController->mRadioDisconnecting);
            }

      };

      class CloudConnected : public Parent::StateBase  {
         public:
            CloudConnected(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "CloudConnected";
            }

            virtual void onEnter() {
               this->mController->resetTimer();
               this->mController->publishStartTime = millis();
               {
                  const size_t messageSize = 100;
                  char message[messageSize+1] = {0};
                  uint32_t connectTime = (this->mController->publishStartTime - this->mController->connectStartTime) / 1000;
                  auto signal = this->mController->mRadio.RSSI();
                  snprintf(message, messageSize, "online|%" PRIu32 "|%d|%d",
                           connectTime,
                           signal.rssi,
                           signal.qual);
                  BT_CORE_LOG_INFO("b cloud.publish(%s)", message);
                  bool ack = this->mController->mCloud.publish(this->mController->mEventNameStatus, message, WITH_ACK);
                  BT_CORE_LOG_INFO("e cloud.publish(online) => %d", ack);
               }
               while(!this->mController->mExecutors.empty()) {
                  auto executor = this->mController->mExecutors.front();
                  this->mController->mExecutors.pop_front();
                  executor(this->mController->mCloud);
               }
               {
                  const size_t messageSize = 100;
                  char message[messageSize+1] = {0};
                  uint32_t publishTime = (millis() - this->mController->publishStartTime) / 1000;
                  snprintf(message, messageSize, "offline|%" PRIu32,
                           publishTime);
                  BT_CORE_LOG_INFO("b cloud.publish(%s)", message);
                  bool ack = this->mController->mCloud.publish(this->mController->mEventNameStatus, message, WITH_ACK);
                  BT_CORE_LOG_INFO("e cloud.publish(offline) => %d", ack);
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
      
      uint32_t connectStartTime;
      uint32_t publishStartTime;

};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Cloud__h
