#include "VL53L1GpioInterface.hpp"
#include "GpioInterface.hpp"
#include <Arduino.h>

VL53L1GpioInterface::~VL53L1GpioInterface() {
  end();
}

VL53L1GpioInterface::VL53L1GpioInterface(TwoWire *const i2cDevice,
    GpioInterface &xshutdownPin) : VL53L1(i2cDevice, -1), xshutdown(xshutdownPin) {
}

void VL53L1GpioInterface::VL53L1_SetDeviceAddressValue(
    std::uint8_t const newValue) {
  VL53L1_SetDeviceAddress(newValue << 1);
}

std::uint8_t VL53L1GpioInterface::VL53L1_GetDeviceAddressValue() const {
  return MyDevice.I2cDevAddr >> 1;
}

VL53L1_Error VL53L1GpioInterface::initSensorWithAddressValue(
    std::uint8_t const addressValue) {
  return InitSensor(addressValue << 1);
}

void VL53L1GpioInterface::VL53L1_Off() {
  digitalWrite(xshutdown, LOW);
  delay(10);
}

int VL53L1GpioInterface::end() {
  pinMode(xshutdown, INPUT);
  return 0;
}

void VL53L1GpioInterface::VL53L1_On() {
  digitalWrite(xshutdown, HIGH);
  delay(10);
}

int VL53L1GpioInterface::begin() {
  pinMode(xshutdown, OUTPUT);
  return 0;
}
