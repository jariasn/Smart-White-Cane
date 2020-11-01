# Smart-White-Cane

The Libraries used in this project are listed below with their respective repository sources.

TMRpcm
https://github.com/TMRh20/TMRpcm

SD
https://github.com/arduino-libraries/SD

How it works:

When turned on, a sound (“h.wav”) is played.

It reads the position of the potentiometer (between values of 1 and 300 cm) and establishes the sound that should be output, informing the position of the potentiometer.

If there is water, the "w.wav" sound is activated and moves backwards.

It reads the input voltage from pin A1 and if the charge button is pressed, the respective audio is played.

If the sensor on the right is activated, the distance is less than the previously set potentiometer value, and in turn is greater than 0, then it moves to the left.

If the middle sensor is activated, it moves backwards.

If the one on the left is activated, it moves to the right.

If a tier or something is detected that triggers the lower sensor, then it moves back.

If there is something below 2m and the upper sensor is activated, then it moves backwards and the “u.wav” sound is played.

If all the distances are greater than the value of the potentiometer or are equal to 0, then all the motors are switched off.

If the main battery runs out, the switch can be moved to use the secondary battery.
