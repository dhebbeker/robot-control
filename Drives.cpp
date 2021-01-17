#include "Drives.hpp"

namespace drives
{

template<int A, int B, int C> std::uint16_t volatile Drive<A, B, C>::counter = 0;
template<int A, int B, int C> std::uint16_t Drive<A, B, C>::target = 0;
template<int A, int B, int C> bool Drive<A, B, C>::isIdle = true;

ICACHE_RAM_ATTR void odometryCounter()
{
  LeftDrive::evaluateInterval();
  RightDrive::evaluateInterval();
}

static Amplitude calcRightSpeed(const Amplitude leftSpeed)
{
  constexpr double calibrationFraction = 25.0 / (maxAmplitude - 350.0);
  constexpr float a = 1.0 + calibrationFraction;
  constexpr float b = maxAmplitude * calibrationFraction;
  return a*leftSpeed - b;
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
}
