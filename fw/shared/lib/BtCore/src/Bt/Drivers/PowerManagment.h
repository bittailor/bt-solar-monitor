//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Drivers::PowerManagment
//
//*************************************************************************************************

#ifndef INC__Bt_Drivers_PowerManagment__h
#define INC__Bt_Drivers_PowerManagment__h

namespace Bt {
namespace Drivers {

class PowerManagment
{
   public:
      PowerManagment() = default;
      PowerManagment(const PowerManagment&) = delete;
      PowerManagment& operator=(const PowerManagment&) = delete;

      void enableCharging();
      void disableCharging();
};

} // namespace Drivers
} // namespace Bt

#endif // INC__Bt_Drivers_PowerManagment__h
