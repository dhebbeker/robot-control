#include "board.hpp"
#include "../utils/array.hpp"
#include "Drives.hpp"
#include <assert.h>
#include <Arduino.h>
#include <cstddef>

namespace board {

struct DistanceSensor
{
  VL53L1GpioInterface sensor;
  float angle; //!< clockwise positive in degrees
};

static constexpr std::uint8_t debugLed = D0;
static constexpr std::uint8_t scl = D3;
static constexpr std::uint8_t sda = D4;
static constexpr std::uint8_t ioExpanderIntB = D7;
static constexpr std::uint8_t ioExpanderIntAInv = D8;
static constexpr std::uint8_t ioExpander1Address = 0x20;
MCP23017 ioExpander1(ioExpander1Address);
static MCP23017Pin VL53L1_1_INT(ioExpander1, 0);
static MCP23017Pin VL53L1_1_XSHUT(ioExpander1, 1);
static MCP23017Pin VL53L1_2_INT(ioExpander1, 2);
static MCP23017Pin VL53L1_2_XSHUT(ioExpander1, 3);
static MCP23017Pin VL53L1_3_INT(ioExpander1, 4);
static MCP23017Pin VL53L1_3_XSHUT(ioExpander1, 5);
static MCP23017Pin VL53L1_4_INT(ioExpander1, 6);
static MCP23017Pin VL53L1_4_XSHUT(ioExpander1, 7);
static MCP23017Pin debugLedGreen(ioExpander1, 8+2);
static MCP23017Pin debugLedYellow(ioExpander1, 8+3);
static MCP23017Pin debugLedRed(ioExpander1, 8+4);
static MCP23017Pin debugLedWhite(ioExpander1, 8+5);
static MCP23017Pin leftBumper(ioExpander1, 8+6);
static MCP23017Pin rightBumper(ioExpander1, 8+7);
static DistanceSensor distanceSensor1 = { .sensor=VL53L1GpioInterface(&Wire, VL53L1_1_XSHUT), .angle=0 };
static DistanceSensor distanceSensor4 = { .sensor=VL53L1GpioInterface(&Wire, VL53L1_4_XSHUT), .angle=90};
DistanceSensor* const distanceSensors[] = {
		&distanceSensor1,
		&distanceSensor4,
	};
static_assert(size(distanceSensors) == numberOfDistanceSensors, "number of initializers incorrect");

static PolarVector distances[size(distanceSensors)] { };

static Distance retrieveSensorStatus(VL53L1GpioInterface& sensor)
{
  uint8_t newDataReady = 0;
  const VL53L1_Error status_getReady = sensor.VL53L1_GetMeasurementDataReady(&newDataReady);
  Distance newDistance = distanceErrorValue;

  if ((status_getReady == VL53L1_ERROR_NONE) && (newDataReady != 0))
  {
    VL53L1_RangingMeasurementData_t rangingMeasurement = VL53L1_RangingMeasurementData_t();
    const VL53L1_Error status_GetData = sensor.VL53L1_GetRangingMeasurementData(&rangingMeasurement);
    sensor.VL53L1_ClearInterruptAndStartMeasurement();
    if (status_GetData == VL53L1_ERROR_NONE && rangingMeasurement.RangeStatus == 0)
    {
      newDistance = rangingMeasurement.RangeMilliMeter;
    }
  }
  return newDistance;
}

static void testDebugLeds()
{
  MCP23017Pin *const debugLeds[] =
  { &debugLedGreen, &debugLedYellow, &debugLedRed, &debugLedWhite };
  for (auto i = 0; i < 3; ++i)
  {
    for (auto led : debugLeds)
    {
      digitalWrite(*led, HIGH);
      delay(125);
      digitalWrite(*led, LOW);
    }
  }
}

/**
 *
 * @param sensor sensor to read from
 * @param[out] returnDistance distance is written into
 * @retval true in case distance was read without error
 */
static bool retrieveSensorStatusOrError(VL53L1GpioInterface& sensor, Distance& returnDistance, const std::size_t maxNumberOfAttempts = 3)
{
  for (std::size_t i = 0; i < maxNumberOfAttempts; ++i)
  {
    returnDistance = retrieveSensorStatus(sensor);
    if (returnDistance != distanceErrorValue)
    {
      return true;
    }
    else
    {
      delay(100);
    }
  }
  return false;
}

bool retrieveSensorStatusOrError(const DistanceSensorIndex sensorIndex, PolarVector& returnDistance, const std::size_t maxNumberOfAttempts)
{
  auto& distanceSensor = *distanceSensors[static_cast<std::size_t>(sensorIndex)];
  returnDistance.angle = distanceSensor.angle;
  return retrieveSensorStatusOrError(distanceSensor.sensor, returnDistance.length, maxNumberOfAttempts);
}

void testDistanceSensors()
{
  Serial.print("Sensor values are: ");
  for (std::size_t i = 0; i < size(distanceSensors); ++i)
  {
    Distance distance = distanceErrorValue;
    const bool isError = !retrieveSensorStatusOrError(distanceSensors[i]->sensor, distance);
    if (isError)
    {
      Serial.printf("Sensor %u = ERROR, \t", i);
    }
    else
    {
      Serial.printf("Sensor %u = %umm, \t", i, distance);
    }
  }
  Serial.println("");
}

void setup(const InterruptFunctionPointer interruptForBumper)
{
  Wire.begin(sda, scl);
  ioExpander1.init();
  ioExpander1.interruptMode(MCP23017InterruptMode::Separated);
  ioExpander1.interrupt(MCP23017Port::B, FALLING);

  // initialize pins
  pinMode(debugLed, OUTPUT);
  digitalWrite(debugLed, LOW);
  pinMode(ioExpanderIntB, INPUT_PULLUP);
  pinMode(ioExpanderIntAInv, INPUT);
  pinMode(VL53L1_1_INT, INPUT_PULLUP);
  pinMode(VL53L1_2_INT, INPUT_PULLUP);
  pinMode(VL53L1_3_INT, INPUT_PULLUP);
  pinMode(VL53L1_4_INT, INPUT_PULLUP);

  // shut down all distance sensors, especially those which are not used later
  pinMode(VL53L1_1_XSHUT, OUTPUT);
  digitalWrite(VL53L1_1_XSHUT, LOW);
  pinMode(VL53L1_2_XSHUT, OUTPUT);
  digitalWrite(VL53L1_2_XSHUT, LOW);
  pinMode(VL53L1_3_XSHUT, OUTPUT);
  digitalWrite(VL53L1_3_XSHUT, LOW);
  pinMode(VL53L1_4_XSHUT, OUTPUT);
  digitalWrite(VL53L1_4_XSHUT, LOW);

  pinMode(debugLedGreen, OUTPUT);
  digitalWrite(debugLedGreen, LOW);
  pinMode(debugLedYellow, OUTPUT);
  digitalWrite(debugLedYellow, LOW);
  pinMode(debugLedRed, OUTPUT);
  digitalWrite(debugLedRed, LOW);
  pinMode(debugLedWhite, OUTPUT);
  digitalWrite(debugLedWhite, LOW);
  pinMode(leftBumper, INPUT_PULLUP);
  pinMode(rightBumper, INPUT_PULLUP);

  testDebugLeds();
  drives::initDrives();

  // initialize distance measurement sensors
  for(std::size_t i=0; i<size(distanceSensors); i++)
  {
    distanceSensors[i]->sensor.begin();
    distanceSensors[i]->sensor.VL53L1_Off();
  }
  for(std::size_t i=0; i<size(distanceSensors); i++)
  {
    Serial.printf("Initialize distance sensor %u...", i);
    assert(distanceSensors[i]->sensor.initSensorWithAddressValue(VL53L1GpioInterface::defaultAddressValue + 1 + i) == VL53L1_ERROR_NONE);
    Serial.printf("Done.\n");
  }
  for(std::size_t i=0; i<size(distanceSensors); i++)
  {
    distanceSensors[i]->sensor.VL53L1_SetPresetMode(VL53L1_PRESETMODE_RANGING);
    distanceSensors[i]->sensor.VL53L1_ClearInterruptAndStartMeasurement();
  }

  if(interruptForBumper != nullptr)
  {
    attachInterrupt(ioExpanderIntB, interruptForBumper, FALLING);
  }

  testDistanceSensors();
}

void loop()
{
  for(std::size_t i=0; i<size(distanceSensors); ++i)
  {
    distances[i].angle = distanceSensors[i]->angle;
    distances[i].length = retrieveSensorStatus(distanceSensors[i]->sensor);
  }
}

bool isBumperPressed()
{
  return digitalRead(leftBumper) == LOW || digitalRead(rightBumper) == LOW;
}

void setDebugLed(const std::uint8_t value, const DebugLeds led)
{
  switch (led)
  {
  case DebugLeds::built_in:
    digitalWrite(debugLed, value);
    break;
  case DebugLeds::green:
    digitalWrite(debugLedGreen, value);
    break;
  case DebugLeds::yellow:
    digitalWrite(debugLedYellow, value);
    break;
  case DebugLeds::red:
    digitalWrite(debugLedRed, value);
    break;
  case DebugLeds::blue:
    digitalWrite(debugLedWhite, value);
    break;
  default:
    break;
  }
}


const PolarVector (&getDistances())[numberOfDistanceSensors]
{
  return distances;
}

}
