#if !defined(DRIVES_HPP)
#define DRIVES_HPP

#include <cstdint>
#include <cmath>
#include "board.hpp"

namespace drives
{

using Counter = std::uint16_t;
using Amplitude = std::uint16_t;
constexpr Amplitude maxAmplitude = 1023;
constexpr Amplitude cruiseSpeed = maxAmplitude / 2;

void odometryCounter();

template<int MotorControl, int DirectionPin, int OdoPin>
struct Drive
{
  
  static Counter volatile counter;
  static Counter target;
  static bool isIdle;

  static void init()
  {
    target = counter = 0;
    pinMode(OdoPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(OdoPin), odometryCounter, CHANGE);
    
    //pinMode(MotorControl, OUTPUT);
    analogWrite(MotorControl, 0);
    pinMode(DirectionPin, OUTPUT);
  }
  
  static void evaluateInterval()
  {
    const bool isContact = digitalRead(OdoPin) == LOW;
    static bool wasContact = isContact;
    static Milliseconds intervalStart = millis(); 
  
    if(wasContact && !isContact) // start interval 
    {
      intervalStart = millis();
    }
    else if(!wasContact && isContact && (millis() - intervalStart) > board::odoMinIntervalDuration)
    {
      // valid interval end
      counter++;
      if(counter>=target) stop();
    }
    
    wasContact = isContact;
  }

  static void stop()
  {
    analogWrite(MotorControl, 0);  
    isIdle = true;  
  }

  static void drive(const Counter distance, const Amplitude amplitude, const bool backwards)
  {
    counter = 0;
    target = distance;
    isIdle = false;
    digitalWrite(DirectionPin, backwards ? LOW : HIGH);
    analogWrite(MotorControl, std::min(maxAmplitude, amplitude));
  }

};

using LeftDrive = Drive<board::leftMotor, board::leftBackwards, board::leftOdoSignal>;
using RightDrive = Drive<board::rightMotor, board::rightBackwards, board::rightOdoSignal>;

void rotateCounter(const Counter deg, const Amplitude amplitude, bool const clockwise);
void rotate(const float deg, const Amplitude amplitude, bool const clockwise);

void driveCounter(const Counter distance, const Amplitude amplitude, const bool backwards);
void drive(const float distance, const Amplitude amplitude, const bool backwards);

};

#endif // DRIVES_HPP
