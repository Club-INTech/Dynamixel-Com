# Dynamixel Communication Library #

This library was developed with a view to using the Dynamixel XL430 servomotor in a robot.


## Supported boards
| Board | using HardwareSerial | using SoftwareSerial |
|---|:---:|:---:|
|Teensy 3.x | natively with [TeensyDuino](https://www.pjrc.com/teensy/teensyduino.html) libs | natively with [TeensyDuino](https://www.pjrc.com/teensy/teensyduino.html) libs|
| STM32 nucleo | not supported | natively with [STM32duino core](https://github.com/stm32duino/Arduino_Core_STM32) libs since v1.8 |
| ESP32 | not supported | adding [ESPSoftwareSerial](https://github.com/plerup/espsoftwareserial) lib |
| Arduino AVR | not supported | adding [SoftwareSerialWithHalfDuplex](https://github.com/nickstedman/SoftwareSerialWithHalfDuplex) lib |
