

This is a simple I2C utility sketch for Arduino.

This software:

- Collects a list of active I2C devices on the bus at startup
- Determines whether any devices respond to the I2C "ALL CALL" address 
- Periodically pings the collected list to determine whether any have stopped responding

It generates debug output on the serial port at 9600 baud: You can connect to your arduino's 
serial port using eg "screen" on Mac OS X or Linux, or any serial terminal emulator.
