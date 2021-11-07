#include "Drives.hpp"
#include "board.hpp"
#include <cmath>

#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231
#if defined(abs)
#undef abs // else it conflicts with std::abs
#endif

namespace board
{
extern constexpr std::uint8_t leftMotor = D1;
extern constexpr std::uint8_t rightMotor = D2;
extern constexpr std::uint8_t rightOdoSignal = D5;
extern constexpr std::uint8_t leftOdoSignal = D6;
MCP23017Pin leftBackwards(ioExpander1, 8 + 0);
MCP23017Pin rightBackwards(ioExpander1, 8 + 1);
}

namespace drives
{

static constexpr float stepsPerRad = stepsPerDeg / ((2.0*numbers::pi)/360.0);

static enum class Action
{
	FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT
} lastAction = Action::FORWARD;

static Position lastKnownPosition = {0,0};
static float orientation = 0; // in radiant

#define TYPEANDSYMBOL(t) decltype(t), t
using LeftDrive = Drive<TYPEANDSYMBOL(board::leftMotor), TYPEANDSYMBOL(board::leftBackwards), TYPEANDSYMBOL(board::leftOdoSignal)>;
using RightDrive = Drive<TYPEANDSYMBOL(board::rightMotor), TYPEANDSYMBOL(board::rightBackwards), TYPEANDSYMBOL(board::rightOdoSignal)>;

template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
Counter volatile Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::counter = 0;
template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
Counter Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::target = 0;
template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
bool Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::isIdle = true;
template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
Milliseconds Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::lastDuration = millis();

/**
 * \name Calibration
 * The right drive tends to be faster than the left. In order to compensate a factor 
 * is applied the the duty cycle of the motor control of the right drive.
 * 
 * In order to estimate the best factor, a calibration was done as follows:
 * 
 *  1. Let the robot drive for a fixed distance.
 *  2. As soon as a drive finishes, it stores the system time in milliseconds.
 *  3. When both drives are finished, the time difference is printed via serial terminal.
 *  4. The factor is determined such as the difference is minimized.
 *  
 * It has been observed that the speed difference between the drive decreases with 
 * increasing speed. Thus a linear function is assumed in order to apply the factor.
 * 
 * As the left drive is slower than the right, the function is only applied to the duty 
 * cycle of the right motor control.
 * 
 * The calibration fraction is defined as follows: It has been observed, that when the 
 * duty cycle of the left is x/1023, then the right should be y/1023 in order for the
 * drives to be equally fast. The linear functions is such that the difference is 0 with 
 * a duty cycle of 100%.
 */
/**@{*/
static float calibrationSlope = 1.033708;
static float calibrationIntercept = -34.483383;

static Amplitude calcRightSpeed(const Amplitude leftSpeed)
{
  return std::max(
                  std::min(
                           std::round(leftSpeed * calibrationSlope + calibrationIntercept),
                           static_cast<float>(maxAmplitude)),
                  static_cast<float>(0));
}

void calibrate(const float testDistance)
{
  while (!isIdle())
  {
    // wait
  }
  constexpr Amplitude testAmplitude = 350;
  const Milliseconds startTime = millis();

  // disable calibration for test
  calibrationSlope = 1;
  calibrationIntercept = 0;

  drive(testDistance, testAmplitude, false);
  while (!isIdle())
  {
    delay(1000); // wait
  }
  Serial.printf("left:%u, right:%u\n", LeftDrive::lastDuration, RightDrive::lastDuration);

  calibrationSlope = (static_cast<double>(RightDrive::lastDuration) / LeftDrive::lastDuration * testAmplitude
      - maxAmplitude) / (testAmplitude - maxAmplitude);
  calibrationIntercept = maxAmplitude * (1 - calibrationSlope);

  Serial.printf(
                "Set calibrationSlope to %f and calibrationIntercept to %f\n",
                calibrationSlope,
                calibrationIntercept);
}
/**@}*/

void rotateCounter(const Counter steps, const Amplitude amplitude, bool const clockwise)
{
  if (steps != 0)
  {
    lastAction = clockwise ? Action::TURN_RIGHT : Action::TURN_LEFT;
    const Amplitude rightSpeed = calcRightSpeed(amplitude);
    LeftDrive::drive(steps, amplitude, clockwise);
    RightDrive::drive(steps, rightSpeed, !clockwise);
  }
}

void driveCounter(const Counter steps, const Amplitude amplitude, const bool backwards)
{
	lastAction = backwards ? Action::BACKWARD : Action::FORWARD;
  const Amplitude rightSpeed = calcRightSpeed(amplitude);
  LeftDrive::drive(steps, amplitude, backwards);
  RightDrive::drive(steps, rightSpeed, backwards);
}

void rotate(const float deg, const Amplitude amplitude)
{
  const bool clockwise = deg > 0;
  const Counter steps = std::round(std::abs(deg) * stepsPerDeg);
  rotateCounter(steps, amplitude, clockwise);
}

void drive(const float distance, const Amplitude amplitude, const bool backwards)
{
  constexpr float stepsPerMm = 1 / odoIntervalLength;
  const Counter steps = distance * stepsPerMm;
  driveCounter(steps, amplitude, backwards);
}

Position flushCurrentPosition()
{
	switch (lastAction) {
		case Action::FORWARD:
		case Action::BACKWARD:
		{
			const std::int8_t reversed = (lastAction == Action::BACKWARD) ? -1 : 1;
			lastKnownPosition.x += LeftDrive::counter * odoIntervalLength * std::cos(orientation) * reversed;
			lastKnownPosition.y += LeftDrive::counter * odoIntervalLength * std::sin(orientation) * reversed;
			break;
		}
		case Action::TURN_LEFT:
		{
			orientation -= LeftDrive::counter / stepsPerRad;
			break;
		}
		case Action::TURN_RIGHT:
		{
			orientation += LeftDrive::counter / stepsPerRad;
			break;
		}
		default:
			break;
	}
	LeftDrive::counter = 0;
	RightDrive::counter = 0;
	return lastKnownPosition;
}

IRAM_ATTR void stopDrives()
{
	LeftDrive::stop();
	RightDrive::stop();
}

bool isIdle()
{
  return LeftDrive::isIdle && RightDrive::isIdle;
}

void initDrives()
{
  LeftDrive::init();
  RightDrive::init();
}

}


