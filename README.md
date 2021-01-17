Calibration of drives
=====================

The right drive tends to be faster than the left. In order to compensate a factor is applied the the duty cycle of the motor control of the right drive.

In order to estimate the best factor, a calibration is done as follows:

 1. Let the robot drive for a fixed distance.
 2. As soon as a drive finishes, it stores the system time in milliseconds.
 3. When both drives are finished, the time difference is printed via serial terminal.
 4. The factor is determined such as the difference is minimized.

It has been observed that the speed difference between the drive decreases with increasing speed. Thus a linear function is assumed in order to apply the factor.

As the left drive is slower than the right, the function is only applied to the duty cycle of the right motor control : `calcRightSpeed()`.

The `calibrationFraction` is defined as follows: It has been observed, that when the duty cycle of the left is 350/1023, then the right should be 325/1023. The linear functions is such that the difference is 0 with a duty cycle of 100%.