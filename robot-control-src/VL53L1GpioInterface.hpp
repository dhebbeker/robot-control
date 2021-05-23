#ifndef VL53L1GPIOINTERFACE_HPP_
#define VL53L1GPIOINTERFACE_HPP_

#include <vl53l1_class.h>
#include <cstdint>

class TwoWire;
class GpioInterface;

class VL53L1GpioInterface : public VL53L1 {
public:
  /** Calls end(). */
  virtual ~VL53L1GpioInterface();

  /**
   * Sets default values for members.
   * @param i2cDevice Is the I2C interface to be used.
   *                  It must be initialized and started.
   * @param xshutdownPin Is the digital output connected to XSHUT.
   */
  VL53L1GpioInterface(TwoWire * const i2cDevice, GpioInterface& xshutdownPin);

  /** The 7 bits of the default address  value right aligned. */
  static constexpr std::uint8_t defaultAddressValue = VL53L1_DEFAULT_DEVICE_ADDRESS >> 1;

  /**
   * Initializes the sensor and sets it's address.
   * @param addressValue 7 bits of the address right aligned.
   * @return VL53L1_ERROR_NONE on success
   */
  virtual VL53L1_Error initSensorWithAddressValue(const std::uint8_t addressValue = defaultAddressValue);

  /**
   * Sets the address of the sensor.
   * @param newValue is the 7 bits of the address value right aligned.
   */
  virtual void VL53L1_SetDeviceAddressValue(const std::uint8_t newValue);

  /**
   * Gets the address which has been written to the sensor.
   *
   * @return The 7 bits of the address value right-aligned.
   */
  virtual std::uint8_t VL53L1_GetDeviceAddressValue() const;

  /**
   * Sets the mode of the xshutdownPin to OUTPUT.
   * @return 0
   */
  virtual int begin() override;

  /**
   * Sets the mode of XShutdown pin to INPUT.
   * @return 0
   */
  virtual int end() override;

  /**
   * Switches on the sensor.
   *
   * The digital output connected to XSHUT is set to high.
   */
  virtual void VL53L1_On() override;

  /**
   * Switches off the sensor.
   *
   * The digital output connected to XSHUT is set to low.
   */
  virtual void VL53L1_Off() override;

protected:
  GpioInterface& xshutdown;
};

#endif /* VL53L1GPIOINTERFACE_HPP_ */
