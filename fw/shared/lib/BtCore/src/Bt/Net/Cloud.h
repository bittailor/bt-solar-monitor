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
#include <Bt/Core/Timer.h>

namespace Bt {
namespace Net {

template<typename TRadio, typename TClient>
class CloudState {
   public:
      virtual ~CloudState() {}
      virtual void executeConnected(std::function<bool (TClient&)> pExecutor){};
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



      static constexpr auto cEventNameOnline = "bt/device/state/online"; 
      static constexpr auto cEventNameOffline = "bt/device/state/offline"; 

      typedef Core::StateMachine<CloudState<TRadio,TClient>, Cloud<TRadio,TClient> > Parent;
      typedef TClient Client;

      Cloud(TRadio& pRadio, TClient& pCloud)
      : mRadio(pRadio)
      , mCloud(pCloud)
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
         bool success = mCloud.function("Cloud-updateOnlinePeriod", &Cloud::updateOnlinePeriod, this);
         BT_CORE_LOG_INFO("mCloud.function(stayOnlineTime) => %d", success);
         this->init(mInitial);         
      }

      void executeConnected(std::function<bool (TClient&)> pExecutor) {
         this->handle(&CloudState<TRadio,TClient>::executeConnected, pExecutor);
      }

      virtual void OnSystemEvent(system_event_t pEvent, int pParam) {
         this->handle(&CloudState<TRadio,TClient>::OnSystemEvent, pEvent, pParam);
      }

      int updateOnlinePeriod(String data) {
         int onlinePeriod = data.toInt();
         BT_CORE_LOG_INFO("mCloud.updateOnlinePeriod(%s) => %d", data.c_str(), onlinePeriod);
         mOnlineTimer = Core::Timer(onlinePeriod * 1000);
         return onlinePeriod;
      }

      void handleCmd(const char *pEventName, const char *pData) {
         BT_CORE_LOG_ERROR("handleCmd (%s %s)", pEventName, pData); 

         int seconds = 0;
         
         JSONValue outerObj = JSONValue::parseCopy(pData);
         JSONObjectIterator iter(outerObj);
         while(iter.next()) {
            BT_CORE_LOG_INFO("key=%s value=%s", 
               (const char *) iter.name(), 
               (const char *) iter.value().toString());
            if(iter.name() == "second") {
               seconds = iter.value().toInt();   
            }
         }
         BT_CORE_LOG_INFO(" mOnlineTimer = %d ",seconds);
         mOnlineTimer = Core::Timer(seconds * 1000); 
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

            virtual void executeConnected(std::function<bool (TClient&)> pExecutor) {
               this->mController->mConnectStartTime = millis();
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

            virtual void executeConnected(std::function<bool (TClient&)> pExecutor) {
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

            virtual void executeConnected(std::function<bool (TClient&)> pExecutor) {
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
            CloudConnected(Cloud& pController):Parent::StateBase(pController){
            }

            virtual const char* name() {
               return "CloudConnected";
            }

            virtual void onEnter() {
               this->mController->resetTimer();
               this->mController->mPublishStartTime = millis();
               this->mController->mOnlineTimer = Core::Timer(1000*5); // 1000*30
               {
                  uint32_t timeToConnect = (this->mController->mPublishStartTime - this->mController->mConnectStartTime) / 1000;
                  auto signal = this->mController->mRadio.RSSI();
                  const size_t messageSize = 100;
                  char message[messageSize+1] = {0};
                  JSONBufferWriter writer(message, sizeof(message));
                  writer.beginObject();
                     writer.name("timeToConnect").value(timeToConnect);
                     writer.name("signal").beginObject();
                        writer.name("strength").value(signal.getStrength());
                        writer.name("quality").value(signal.getQuality());
                     writer.endObject();
                  writer.endObject();
                  BT_CORE_LOG_INFO("b cloud.publish(%s)", writer.buffer());
                  bool ack = this->mController->mCloud.publish(cEventNameOnline, writer.buffer(), WITH_ACK);
                  this->mController->mCloud.publishVitals();
                  BT_CORE_LOG_INFO("e cloud.publish(...) => %d", ack);
               }
               processConnected();
            }

            virtual void onExit() {
               uint32_t onlinePeriod = (millis() - this->mController->mPublishStartTime) / 1000;
               const size_t messageSize = 100;
               char message[messageSize+1] = {0};
               JSONBufferWriter writer(message, sizeof(message));
               writer.beginObject();
               writer.name("onlinePeriod").value(onlinePeriod);
               writer.endObject();
               {
                  BT_CORE_LOG_INFO("b cloud.publish(%s)", writer.buffer());
                  bool ack = this->mController->mCloud.publish(cEventNameOffline, writer.buffer(), WITH_ACK);
                  BT_CORE_LOG_INFO("e cloud.publish(...) => %d", ack);
               }
               this->mController->mCloud.disconnect();
            }

            virtual void executeConnected(std::function<bool (TClient&)> pExecutor) {
               if(!pExecutor(this->mController->mCloud)) {
                  this->mController->mExecutors.push_back(pExecutor);   
               }
            }

            virtual void timeUp(){
               processConnected();   
            }

            void processConnected() {
               BT_CORE_LOG_INFO("processConnected %d ...", this->mController->mExecutors.size());
               bool allDone = runExecutors();
               if(allDone && this->mController->mOnlineTimer.expired()) {
                  BT_CORE_LOG_INFO("... processConnected all done => disconnecting");
                  this->mController->nextState(this->mController->mCloudDisconnecting);
               } else {
                  BT_CORE_LOG_INFO("... processConnected %d left [%" PRIu32 "] => reprocess in 1s", this->mController->mExecutors.size(), this->mController->mOnlineTimer.msTillExpired());
                  this->mController->setTimer(1000);
               }
            }

            bool runExecutors() {
               if(this->mController->mExecutors.empty()) {
                  return true;
               }

               std::deque<std::function<bool (Client&)>> executors;
               executors.swap(this->mController->mExecutors);
               while(!executors.empty()) {
                  auto executor = executors.front();
                  executors.pop_front();
                  if(!executor(this->mController->mCloud)) {
                     this->mController->mExecutors.push_back(executor);   
                  }
               }
               return this->mController->mExecutors.empty();
            }
      };

      class CloudDisconnecting : public Parent::StateBase  {
         public:
            CloudDisconnecting(Cloud& pController):Parent::StateBase(pController){}

            virtual const char* name() {
               return "CloudDisconnecting";
            }

            virtual void executeConnected(std::function<bool (TClient&)> pExecutor) {
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

            virtual void executeConnected(std::function<bool (TClient&)> pExecutor) {
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
      std::deque<std::function<bool (Client&)> > mExecutors;
      std::string mEventNameState;

      Initial mInitial;
      RadioOff mRadioOff;
      RadioConnecting mRadioConnecting;
      CloudConnecting mCloudConnecting;
      CloudConnected mCloudConnected;
      
      CloudDisconnecting mCloudDisconnecting;
      RadioDisconnecting mRadioDisconnecting;

      Core::Timer  mOnlineTimer;
      
      uint32_t mConnectStartTime;
      uint32_t mPublishStartTime;
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Cloud__h
