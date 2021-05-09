#include "Drives.hpp"
#include "board.hpp"

#include <cmath>

#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231

namespace drives
{

template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
Counter volatile Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::counter = 0;
template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
Counter Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::target = 0;
template<typename MOTORCONTROL, MOTORCONTROL &motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN &directionPin, typename ODOPIN, ODOPIN &odoPin>
bool Drive<MOTORCONTROL, motorControlpin, DIRECTIONPIN, directionPin, ODOPIN, odoPin>::isIdle = true;

/**
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
static Amplitude calcRightSpeed(const Amplitude leftSpeed)
{
	constexpr Amplitude leftAmplitude = 350; // x
	constexpr Amplitude rightMatchingAmplitude = 290; // y
	constexpr double calibrationFraction = (leftAmplitude
			- rightMatchingAmplitude)
			/ static_cast<double>(maxAmplitude - leftAmplitude);
	return leftSpeed - calibrationFraction * (maxAmplitude - leftSpeed);
}

void rotateCounter(const Counter steps, const Amplitude amplitude, bool const clockwise)
{
  LeftDrive::drive(steps, amplitude, !clockwise);
  RightDrive::drive(steps, calcRightSpeed(amplitude), clockwise);
}

void driveCounter(const Counter steps, const Amplitude amplitude, const bool backwards)
{
  LeftDrive::drive(steps, amplitude, backwards);
  RightDrive::drive(steps, calcRightSpeed(amplitude), backwards);
}

void rotate(const float deg, const Amplitude amplitude, bool const clockwise)
{
  constexpr float stepsPerDeg = 100/(4.8*360);
  const Counter steps = std::round(deg * stepsPerDeg);
  rotateCounter(steps, amplitude, clockwise);
}

void drive(const float distance, const Amplitude amplitude, const bool backwards)
{
  constexpr float stepsPerMm = 1 / board::odoIntervalLength;
  const Counter steps = distance * stepsPerMm;
  driveCounter(steps, amplitude, backwards);
}

IRAM_ATTR void stopDrives() {
	LeftDrive::stop();
	RightDrive::stop();
}

}


