#include "Drives.hpp"
#include "board.hpp"

#include <cmath>

#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231

template<typename MOTORCONTROL, typename DIRECTIONPIN, typename ODOPIN>
static drives::Drive<MOTORCONTROL, DIRECTIONPIN, ODOPIN> makeDrive(MOTORCONTROL& motorControlPin, DIRECTIONPIN& directionPin, ODOPIN& odoPin)
{
	return drives::Drive<MOTORCONTROL, DIRECTIONPIN, ODOPIN>(motorControlPin, directionPin, odoPin);
}

namespace drives
{
static auto l = makeDrive(board::leftMotor, board::leftBackwards, board::leftOdoSignal);
static auto r = makeDrive(board::rightMotor, board::rightBackwards, board::rightOdoSignal);

DriveInterface& leftDrive = l;
DriveInterface& rightDrive = r;

IRAM_ATTR void odometryCounter()
{
  leftDrive.evaluateInterval();
  rightDrive.evaluateInterval();
}

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
 * duty cycle of the left is 350/1023, then the right should be 325/1023 in order for the
 * drives to be equally fast. The linear functions is such that the difference is 0 with 
 * a duty cycle of 100%.
 */
static Amplitude calcRightSpeed(const Amplitude leftSpeed)
{
  constexpr double calibrationFraction = 25.0 / (maxAmplitude - 350.0);
  constexpr float a = 1.0 + calibrationFraction;
  constexpr float b = maxAmplitude * calibrationFraction;
  return a*leftSpeed - b;
}

void rotateCounter(const Counter steps, const Amplitude amplitude, bool const clockwise)
{
  leftDrive.drive(steps, amplitude, !clockwise);
  rightDrive.drive(steps, calcRightSpeed(amplitude), clockwise);
}

void driveCounter(const Counter steps, const Amplitude amplitude, const bool backwards)
{
  leftDrive.drive(steps, amplitude, backwards);
  rightDrive.drive(steps, calcRightSpeed(amplitude), backwards);
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

DriveInterface::DriveInterface() :
		counter(0), target(0), isIdle(true) {
}

DriveInterface::~DriveInterface() {
}

IRAM_ATTR void stopDrives() {
	leftDrive.stop();
	rightDrive.stop();
}

}


