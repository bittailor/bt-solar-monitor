//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Cloud
//
//*************************************************************************************************

#include "Bt/Net/Cloud.h"

#include <algorithm>

namespace Bt {
namespace Net {

std::vector<CloudBase*> CloudBase::sClouds;

void handleSystemEventForCloud(system_event_t event, int param, void*) {
   BT_CORE_LOG_INFO("SystemEventForCloud %lu - %d send to %d ", (uint32_t)event, param, CloudBase::sClouds.size());
   for(CloudBase* cloud : CloudBase::sClouds) {
      cloud->OnSystemEvent(event, param);
   }
}

void CloudBase::begin() {
   if(CloudBase::sClouds.empty()) {
      System.on(network_status | cloud_status, &handleSystemEventForCloud);
   }
   CloudBase::sClouds.push_back(this);
   BT_CORE_LOG_INFO("CloudBase() %d ", CloudBase::sClouds.size());

}

CloudBase::~CloudBase() {
   CloudBase::sClouds.erase(std::find(std::begin(CloudBase::sClouds), std::end(CloudBase::sClouds), this));
   if(CloudBase::sClouds.empty()) {
      System.off(&handleSystemEventForCloud);
   }
   BT_CORE_LOG_INFO("~CloudBase() %d ", CloudBase::sClouds.size());
}




} // namespace Net
} // namespace Bt
