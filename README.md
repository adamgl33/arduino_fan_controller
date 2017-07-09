# Arduino Fan Controller

### Description
Arduino based computer fan PWM controller for 3 or 4 wire fans with PWM input.

### Setup
* Connect the PWM inputs of the fans to analogue pins on the Arduino.
* Make sure arduino negative and fan negative are connected.
* Configure the fans array with the number of fans required.  One element for each fan.
* Set the initial speed and output pin number for each fan.
* Optional: set other constants like increment, min/max speeds, etc.


### User Manual
* 'Left' and 'Right' buttons select between active fan.
* 'Up' and 'Down' buttons increase and decrease fan speed by a predefined increment, respectively.  Updated speeds are saved in the EEPROM for recovery after a power out or a reset.
* 'Select' button toggles 'Pause' mode.  A 'P' is displayed on the LCD to denote this mode.  Pause mode temporarily sets all fan speeds to zero.  These speeds are not saved in the EEPROM.  Previous fan speeds will be restored either by clicking 'Select' again, or by resetting the Arduino.
* Sleep mode dims LCD backlight after a set amount of time has passed since the last button press.  The amount of time can be configured in the constants section (const unsigned long timeToFade)
