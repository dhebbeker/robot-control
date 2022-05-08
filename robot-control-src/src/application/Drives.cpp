#include "Drives.hpp"
#include "board.hpp"
#include <cmath>
#include <cassert>

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
 * The right drive tends to be faster than the left.
 * The leads to the robot to drive curves.
 * In order to compensate, a factor is applied the the duty cycle of the motor control
 * of the right drive.
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
 * It is assumed, that the right drive speed does deviate from the left drive speed.
 *
 * Also it has been observed that the deviation between the left and right drive speed does
 * decrease down to 0 until it reaches maximum speed.
 * Thus a linear function is assumed for the deviating speed of the right drive.
 *
 * Speed of the left drive:
 *
 * \f[
 *    t_l = \frac{s}{v}
 * \f]
 *
 * And the speed of the right drive:
 *
 * \f[
 *    t_r = \frac{s}{\underbrace{m \cdot v + n}_{\textrm{linear function}}}
 * \f]
 *
 * Where \f$s\f$ is the distance for which the time \f$t\f$ is measured while driving with speed \f$v\f$.
 *
 * Now we measure the times \f$t_r\f$ and \f$t_l\f$ while driving the same distance with two different
 * speeds \f$v_1\f$ and \f$v_2\f$.
 * Resulting in \f$t_{l,1}\f$, \f$t_{l,2}\f$, \f$t_{r,1}\f$, \f$t_{r,2}\f$.
 *
 * Now we can solve the system of the 4 equations for \f$m\f$:
 *
 * \f[
 *    m = \frac{\frac{1}{t_{r,1}}-\frac{1}{t_{r,2}}}{\frac{1}{t_{l,1}}-\frac{1}{t_{l,2}}}
 * \f]
 *
 * Now we define, that for the maximum speed \ref drives::maxAmplitude \f$\hat{v}\f$, the
 * times are the same (which has been observed).
 *
 * \f[
 *    \frac{s}{\hat{v}} = \frac{s}{m \cdot \hat{v} + n}
 * \f]
 *
 * which can be solved for \f$n\f$:
 *
 * \f[
 *    n = \hat{v} \cdot (m -1 )
 * \f]
 *
 * Now \f$m\f$ (\ref calibrationSlope) and \f$n\f$ (\ref calibrationIntercept) can be used to calculate
 * the adjusted speed of the right drive using the speed for the left drive:
 *
 * \f[
 *    v_r = m \cdot v_l + n
 * \f]
 */
/**@{*/
static float calibrationSlope = 0.973500644796801;
static float calibrationIntercept = -27.1088403728729;

static Amplitude calcRightSpeed(const Amplitude leftSpeed)
{
  return std::max(
                  std::min(
                           static_cast<Amplitude>(std::lround(
                                                              leftSpeed * calibrationSlope
                                                                  + calibrationIntercept)),
                           maxAmplitude),
                  static_cast<Amplitude>(0U));
}

void calibrate(const float testDistance)
{
  while (!isIdle())
  {
    // wait
  }
  // disable calibration for test
  calibrationSlope = 1;
  calibrationIntercept = 0;

  // Measure with speed 1
  constexpr Amplitude testAmplitude1 = 350;
  drive(testDistance, testAmplitude1, false);
  while (!isIdle())
  {
    delay(1000); // wait
  }
  const Milliseconds tl1 = LeftDrive::lastDuration;
  const Milliseconds tr1 = RightDrive::lastDuration;

  // Measure with speed 2
  constexpr Amplitude testAmplitude2 = maxAmplitude;
  drive(testDistance, testAmplitude2, true);
  while (!isIdle())
  {
    delay(1000); // wait
  }
  const Milliseconds tl2 = LeftDrive::lastDuration;
  const Milliseconds tr2 = RightDrive::lastDuration;

  calibrationSlope = (1. / tr1 - 1. / tr2) / (1. / tl1 - 1. / tl2);
  calibrationIntercept = testAmplitude2 * (calibrationSlope - 1.);

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
  assert(!std::isnan(deg));
  const bool clockwise = deg > 0;
  const Counter steps = std::lround(std::abs(deg) * stepsPerDeg);
  rotateCounter(steps, amplitude, clockwise);
}

void drive(const float distance, const Amplitude amplitude, const bool backwards)
{
  assert(!std::isnan(distance));
  constexpr float stepsPerMm = 1 / odoIntervalLength;
  const Counter steps = std::lround(distance * stepsPerMm);
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


