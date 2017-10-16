//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::SensorArray
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_SensorArray__h
#define INC__Bt_Sensors_SensorArray__h

#include <array>
#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>
#include <Bt/Core/Sleep.h>

namespace Bt {
namespace Sensors {

template<typename Sensor, typename Reading, std::size_t N>
class SensorArray
{
   public:

      SensorArray(std::array<Sensor,N>& pSensors, unsigned long pPowerUpDelayInMs)
      : mSensors(pSensors), mPowerUpDelayInMs(pPowerUpDelayInMs) {
      }
      SensorArray(const SensorArray&) = delete;
      SensorArray& operator=(const SensorArray&) = delete;

      std::array<Reading,N> readAll() {
         static int sCounter = 0;
         sCounter++;
         BT_CORE_LOG_DEBUG("SensorArray - power up");
         for (Sensor& sensor : mSensors) {
            sensor.powerUp();
         }
         BT_CORE_LOG_DEBUG("SensorArray - up");
         if(sCounter % 2 == 1) {
            Bt::Core::msSleep(mPowerUpDelayInMs);
         } else {
            delay(mPowerUpDelayInMs);
         }

         std::array<Reading,N> readings;
         for(std::size_t i = 0 ; i < N ; i++) {
            readings[i]= mSensors[i].read();
            BT_CORE_LOG_DEBUG("SensorArray[%d] - [%s] U = %f I = %f ",i , readings[i].valid ? "valid" : "invalid"  ,readings[i].busVoltage, readings[i].current);
         }
         BT_CORE_LOG_DEBUG("SensorArray - power down");
         for (Sensor& sensor : mSensors) {
            sensor.powerDown();
         }
         BT_CORE_LOG_DEBUG("SensorArray - down");
         return readings;
      }

   private:
      std::array<Sensor,N>& mSensors;
      unsigned long mPowerUpDelayInMs;
};

} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_SensorArray__h
