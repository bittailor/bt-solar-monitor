//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::INA219
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_INA219__h
#define INC__Bt_Sensors_INA219__h

#include <stdint.h>
#include <stddef.h>

#include <Particle.h>

namespace Bt {
namespace Sensors {

typedef TwoWire I_I2C;

struct INA219Reading {
	bool valid;
	float current;
	float busVoltage;
	float shuntVoltage;
};

class INA219
{
   public:
	  enum class Gain { GAIN_40MV,GAIN_80MV,GAIN_160MV,GAIN_320MV };

      INA219(I_I2C& pI2C, uint8_t pAddress, Gain pGain, uint16_t pCalibrationRegister, uint16_t pCurrentLSB);
      INA219(const INA219&) = delete;
      INA219& operator=(const INA219&) = delete;
      ~INA219();

      void begin();

      void powerUp();
      void powerDown();
      INA219Reading read();

   private:
      
      void reset();
      void configure(bool pValidate = false);
      void calibrate(bool pValidate = false);

      void writeRegister(uint8_t pRegister, uint16_t pValue);
      uint16_t readRegister(uint8_t pRegister);

      I_I2C& mI2C;
      uint8_t mAddress;
      uint16_t mConfigurationRegister;
      uint16_t mCalibrationRegister;
      uint16_t mCurrentLSB;
};

} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_INA219__h
