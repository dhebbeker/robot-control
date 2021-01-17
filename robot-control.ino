#include <cstdint>
#include <cmath>
#include <limits>

typedef decltype(millis()) Milliseconds;
constexpr auto pi = std::acos(-1);

static volatile bool isLeftDriveCalibrationActive = false, isRightDriveCalibrationActive = false;

namespace board
{
constexpr auto leftOdoSignal = D6; //!< blue
constexpr auto rightOdoSignal = D7; //!< green
constexpr auto rightBackwards = D5; //!< yellow
constexpr auto leftBackwards = D3; //!< white
constexpr auto leftMotor = D1; //!< brown
constexpr auto rightMotor = D2; //!< orange
constexpr auto debugLed = D4;

constexpr auto maxVelocity = 220; //!< [mm/s]
constexpr float odoIntervalLength = 12*pi; //!< [mm]
//! Minimum time with buffer factor [ms]
constexpr Milliseconds odoMinIntervalDuration = (odoIntervalLength * 1000.0) / (maxVelocity*2.0);
};

void odometryCounter();

template<int MotorControl, int DirectionPin, int OdoPin>
struct Drive
{
  using Counter = std::uint16_t;
  using Amplitude = std::uint16_t;

  static constexpr Amplitude maxAmplitude = 1023;
  
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

template<int A, int B, int C> std::uint16_t volatile Drive<A, B, C>::counter = 0;
template<int A, int B, int C> std::uint16_t Drive<A, B, C>::target = 0;
template<int A, int B, int C> bool Drive<A, B, C>::isIdle = true;

using LeftDrive = Drive<board::leftMotor, board::leftBackwards, board::leftOdoSignal>;
using RightDrive = Drive<board::rightMotor, board::rightBackwards, board::rightOdoSignal>;

ICACHE_RAM_ATTR void odometryCounter()
{
  static Milliseconds leftTime, rightTime;
  LeftDrive::evaluateInterval();
  RightDrive::evaluateInterval();
  if(isLeftDriveCalibrationActive && LeftDrive::isIdle)
  {
    leftTime = millis();
    isLeftDriveCalibrationActive = false;
  }
  if(isRightDriveCalibrationActive && RightDrive::isIdle)
  {
    rightTime = millis();
    isRightDriveCalibrationActive = false;
  }
  if(!isRightDriveCalibrationActive && !isLeftDriveCalibrationActive)
  {
    Serial.printf("left time - right time = \t%li\n", leftTime - rightTime);
  }
}

static void rotate(const float deg, const LeftDrive::Amplitude amplitude, bool const clockwise)
{
  constexpr float stepsPerDeg = 100/(4.8*360);
  const LeftDrive::Counter steps = std::round(deg * stepsPerDeg);
  LeftDrive::drive(steps, amplitude, !clockwise);
  RightDrive::drive(steps, amplitude, clockwise);
}

static RightDrive::Amplitude calcRightSpeed(const LeftDrive::Amplitude leftSpeed)
{
  constexpr double calibrationFraction = 25.0 / (RightDrive::maxAmplitude - 350.0);
  constexpr float a = 1.0 + calibrationFraction;
  constexpr float b = RightDrive::maxAmplitude * calibrationFraction;
  return a*leftSpeed - b;
}

static void drive(const float distance, const LeftDrive::Amplitude amplitude, const bool backwards)
{
  constexpr float stepsPerMm = 1 / board::odoIntervalLength;
  constexpr float calibrationFraction = 25.0 / 673.0;
  const LeftDrive::Counter steps = distance * stepsPerMm;
  LeftDrive::drive(steps, amplitude, backwards);
  RightDrive::drive(steps, calcRightSpeed(amplitude), backwards);
}

constexpr auto cruiseSpeed = LeftDrive::maxAmplitude / 2;

static void driveCalibration()
{
  isRightDriveCalibrationActive = true;
  isLeftDriveCalibrationActive = true;
  drive(1500, cruiseSpeed, false);
}

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.flush();
  delay(100);
  Serial.printf("\n begin program '%s'\n", __FILE__);
  pinMode(board::debugLed, OUTPUT);
  digitalWrite(board::debugLed, LOW);
  LeftDrive::init();
  RightDrive::init();
  driveCalibration();
}

void loop()
{
  static int sequence = 10; 
  Serial.printf("left: \t%3u, right: \t%3u\n", LeftDrive::counter, RightDrive::counter);
  if(LeftDrive::isIdle && RightDrive::isIdle)
  {
    switch(sequence)
    {
      case 0:
        drive(300, cruiseSpeed, false);
        break;
      case 1:
        rotate(180, cruiseSpeed, true);
        break;
      case 2:
        drive(300, cruiseSpeed, false);
        break;
      default:
        sequence = 100;
        break;
    }
    sequence++;
  }
  delay(1000);
}
