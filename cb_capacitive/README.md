capacitive
==============

Device to send OSC messages when a capacitive sensor is touched. You need an Adafruit MPR121 capaciive hat.

usage 
```
    -c conf
```
for example
```
    capacitive -c /home/pi/cap.xml
```
will start the program with the configuration at the given path.   
if you use it like this:   
```
    capacitive
```
the configuration in `bin/data/settings.xml` is loaded.    

i2c has to be activated on you raspberry pi using `raspi-config`

![alt tag](https://github.com/npisanti/np-clayblocks/blob/master/capacitive/mpr121.jpg)  
