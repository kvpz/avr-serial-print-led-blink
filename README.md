### Serial Communications with the ATMega328p and AVR C Library

#### Running in Unix-like environment
###### How to build the binaries
```
make PRG=flickprint-avr 
``` 
or 
``` 
make PRG=flickprint-minimal-avr 
```
*flickprint-avr* and *flickprint-minimal-avr* are programmed differently but do the same thing. 

If there is an issue building, look at the makefile and verify that the AVR C Library is located at the path assigned to the AVR variable.

###### How to burn firmware
``` 
make PRG=flickprint-avr upload 
```
If that does not work, make sure that the host computer is not already listening on port /dev/ttyACM0. You can see what port the Arduino is connected to by running ```arduino-cli board list```. Only start listening to the port after the firmware has been installed. If the port is different than the one mentioned, replace the port in the makefile with the correct path.
