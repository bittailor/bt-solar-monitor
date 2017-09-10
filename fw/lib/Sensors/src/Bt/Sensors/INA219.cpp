//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::INA219
//
//*************************************************************************************************

#include "Bt/Sensors/INA219.h"
#include "Bt/Core/Log.h"

namespace Bt {
namespace Sensors {

#define INA219_REGISTER_CONFIGURATION    (0x00)
#define INA219_REGISTER_SHUNTVOLTAGE     (0x01)
#define INA219_REGISTER_BUSVOLTAGE       (0x02)
#define INA219_REGISTER_POWER            (0x03)
#define INA219_REGISTER_CURRENT          (0x04)
#define INA219_REGISTER_CALIBRATION      (0x05)

#define INA219_DEFAULT_CONFIGURATION     (0x399F)

#define INA219_BUS_VOLTAGE_RANGE_16V     (0x0000)  // 0-16V Range
#define INA219_BUS_VOLTAGE_RANGE_32V     (0x2000)  // 0-32V Range

#define INA219_GAIN_1_40MV               (0x0000)  // Gain 1, 40mV Range
#define INA219_GAIN_2_80MV               (0x0800)  // Gain 2, 80mV Range
#define INA219_GAIN_4_160MV              (0x1000)  // Gain 4, 160mV Range
#define INA219_GAIN_8_320MV              (0x1800)  // Gain 8, 320mV Range

#define INA219_BADCRES_9BIT              (0x0080)  // 9-bit bus res = 0..511
#define INA219_BADCRES_10BIT             (0x0100)  // 10-bit bus res = 0..1023
#define INA219_BADCRES_11BIT             (0x0200)  // 11-bit bus res = 0..2047
#define INA219_BADCRES_12BIT             (0x0400)  // 12-bit bus res = 0..4097

#define INA219_SADCRES_9BIT_1S_84US      (0x0000)  // 1 x 9-bit shunt sample
#define INA219_SADCRES_10BIT_1S_148US    (0x0008)  // 1 x 10-bit shunt sample
#define INA219_SADCRES_11BIT_1S_276US    (0x0010)  // 1 x 11-bit shunt sample
#define INA219_SADCRES_12BIT_1S_532US    (0x0018)  // 1 x 12-bit shunt sample
#define INA219_SADCRES_12BIT_2S_1060US   (0x0048)  // 2 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_4S_2130US   (0x0050)  // 4 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_8S_4260US   (0x0058)  // 8 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_16S_8510US  (0x0060)  // 16 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_32S_17MS    (0x0068)  // 32 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_64S_34MS    (0x0070)  // 64 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_128S_69MS   (0x0078)  // 128 x 12-bit shunt samples averaged together

#define INA219_MODE_FILTER               (0xFFF8)
#define INA219_MODE_POWERDOWN            (0x0000)
#define INA219_MODE_SVOLT_TRIGGERED      (0x0001)
#define INA219_MODE_BVOLT_TRIGGERED      (0x0002)
#define INA219_MODE_SANDBVOLT_TRIGGERED  (0x0003)
#define INA219_MODE_ADCOFF               (0x0004)
#define INA219_MODE_SVOLT_CONTINUOUS     (0x0005)
#define INA219_MODE_BVOLT_CONTINUOUS     (0x0006)
#define INA219_MODE_SANDBVOLT_CONTINUOUS (0x0007)

#define INA219_TIMING_DELAY_WRITE_READ           ( 4)
#define INA219_TIMING_DELAY_POWER_DOWN_RECOVERY  (40)

namespace{
   uint16_t convertGain(INA219::Gain pGain) {
      switch(pGain) {
         case INA219::Gain::GAIN_40MV : return INA219_GAIN_1_40MV;
         case INA219::Gain::GAIN_80MV : return INA219_GAIN_2_80MV;
         case INA219::Gain::GAIN_160MV : return INA219_GAIN_4_160MV;
         case INA219::Gain::GAIN_320MV : return INA219_GAIN_8_320MV;
         default: return INA219_GAIN_1_40MV;
      }
   }
}

INA219::INA219(I_I2C& pI2C, uint8_t pAddress, Gain pGain, uint16_t pCalibrationRegister, uint16_t pCurrentLSB)
: mI2C(pI2C)
, mAddress(pAddress)
, mConfigurationRegister(INA219_BUS_VOLTAGE_RANGE_16V |
                         convertGain(pGain) |
                         INA219_BADCRES_12BIT |
                         INA219_SADCRES_12BIT_4S_2130US |
                         INA219_MODE_POWERDOWN)
, mCalibrationRegister(pCalibrationRegister)
, mCurrentLSB(pCurrentLSB) {
}

INA219::~INA219() {

}

void INA219::begin() {
   reset();
   calibrate(true);
   configure(true);
}

void INA219::powerUp() {
   mConfigurationRegister = (mConfigurationRegister & INA219_MODE_FILTER) | INA219_MODE_SANDBVOLT_CONTINUOUS;
   calibrate();
   configure();
}

void INA219::powerDown() {
   mConfigurationRegister = (mConfigurationRegister & INA219_MODE_FILTER) | INA219_MODE_POWERDOWN ;
   configure();
}

INA219Reading INA219::read() {
   int loopGuard = 10;
   while(loopGuard > 0) {
      uint16_t busvoltageRegister = readRegister(INA219_REGISTER_BUSVOLTAGE);
      int16_t conversionReady = (busvoltageRegister & 0x0002)>>1;
      BT_CORE_LOG_DEBUG("INA219 [0x%02x] - checkForReading busvoltageRegister = 0x%04x Conversion Ready = %d ", mAddress, busvoltageRegister, conversionReady);
      if(conversionReady == 1) {
         int16_t rawCurrent = readRegister(INA219_REGISTER_CURRENT);
         int16_t rawShuntVoltage = readRegister(INA219_REGISTER_SHUNTVOLTAGE);
         int16_t rawBusVoltage =  (int16_t)((busvoltageRegister >> 3) * 4);

         float current = rawCurrent;
         current = current / (( 1000 / mCurrentLSB ) * 1000);
         float shuntVoltage = rawShuntVoltage * 0.01;
         float busVoltage = rawBusVoltage * 0.001;
         return INA219Reading{true, current, busVoltage, shuntVoltage};
      }
      loopGuard--;
      BT_CORE_LOG_WARN("INA219 [0x%02x] - not ready yet %d", mAddress, loopGuard);
      delay(1);
   }
   return INA219Reading{false};
}

void INA219::reset() {
   writeRegister(INA219_REGISTER_SHUNTVOLTAGE,0x5555);
   writeRegister(INA219_REGISTER_CONFIGURATION,0x8000);
   delayMicroseconds(INA219_TIMING_DELAY_POWER_DOWN_RECOVERY);
   uint16_t configurationRegister = readRegister(INA219_REGISTER_CONFIGURATION);
   if(configurationRegister != INA219_DEFAULT_CONFIGURATION) {
      BT_CORE_LOG_WARN("INA219 [0x%02x] - REGISTER_CONFIGURATION wrong after reset (0x%04x!=0x%04x)", mAddress, configurationRegister, INA219_DEFAULT_CONFIGURATION);
   } else {
      BT_CORE_LOG_DEBUG("INA219 [0x%02x] - reset ok", mAddress);
   }
}

void INA219::configure(bool pValidate) {
   writeRegister(INA219_REGISTER_CONFIGURATION,mConfigurationRegister);
   if(pValidate) {
      delayMicroseconds(INA219_TIMING_DELAY_WRITE_READ);
      uint16_t configurationRegister = readRegister(INA219_REGISTER_CONFIGURATION);
      if(configurationRegister != mConfigurationRegister) {
         BT_CORE_LOG_WARN("INA219 [0x%02x] - REGISTER_CONFIGURATION wrong after configure (0x%04x!=0x%04x)", mAddress, configurationRegister, mConfigurationRegister);
      } else {
         BT_CORE_LOG_DEBUG("INA219 [0x%02x] - configure ok (0x%04x)", mAddress, mConfigurationRegister);
      }
   }

}

void INA219::calibrate(bool pValidate) {
   writeRegister(INA219_REGISTER_CALIBRATION,mCalibrationRegister);
   if(pValidate) {
      delayMicroseconds(INA219_TIMING_DELAY_WRITE_READ);
      uint16_t calibrationRegister = readRegister(INA219_REGISTER_CALIBRATION);
      if(calibrationRegister != mCalibrationRegister) {
         BT_CORE_LOG_WARN("INA219 [0x%02x] - INA219_REGISTER_CALIBRATION wrong after calibrate (0x%04x!=0x%04x)", mAddress, calibrationRegister, mCalibrationRegister);
      } else {
         BT_CORE_LOG_DEBUG("INA219 [0x%02x] - calibrate ok (0x%04x)", mAddress, mCalibrationRegister);
      }
   }
}

void INA219::writeRegister(uint8_t pRegister, uint16_t pValue)
{
   mI2C.beginTransmission(mAddress);
   mI2C.write(pRegister);
   mI2C.write((pValue >> 8) & 0xFF);
   mI2C.write(pValue & 0xFF);
   mI2C.endTransmission();
}

uint16_t INA219::readRegister(uint8_t pRegister)
{
   mI2C.beginTransmission(mAddress);
   mI2C.write(pRegister);
   mI2C.endTransmission();
   mI2C.requestFrom(mAddress, static_cast<uint8_t>(2));
   return ((mI2C.read() << 8) | mI2C.read());
}

} // namespace Sensors
} // namespace Bt


