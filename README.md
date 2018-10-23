clayblocks
==============
`clayblocks` is a collection sketches/prototypes of devices that send and receive OSC messages over the network. It includes both android based controllers and raspberry pi based devices that convert sensors reading to OSC messages or control actuators by receiving OSC messages. 

The android sketches are written in [processing](https://processing.org/) with the [oscP5](http://www.sojamo.de/libraries/oscP5/) library. For the phone accelerometer you also need the [ketai](http://ketai.org/) library.

The raspberry pi sketches are openFrameworks apps, so you need [openFrameworks](https://openframeworks.cc/), and also [ofxGPIO](https://github.com/kashimAstro/ofxGPIO) and ofxOsc (included in oF).

A piezo sensor device is avaible on a separated repository, called [ofxRPiezos](https://github.com/npisanti/ofxRPiezos).

Go into the individual folders for usage and pictures.

Here are some test videos:

clayblocks accelerometer:   
[![clayblocks accelerometer](https://i.vimeocdn.com/video/655229823_640.jpg)](https://vimeo.com/233808963 "np-clayblocks accelerometer")

clayblocks keys:   
[![clayblocks keys](https://i.vimeocdn.com/video/492707222_640.jpg)](https://vimeo.com/108844525 "np-clayblocks keys")

clayblocks xy control:   
[![clayblocks xy control](https://i.vimeocdn.com/video/492704537_640.jpg)](https://vimeo.com/108881498 "np-clayblocks xy control")

capacitive touch device: 
![alt tag](https://github.com/npisanti/np-clayblocks/blob/master/capacitive/mpr121.jpg)  

relays controller: 
![alt tag](https://github.com/npisanti/np-clayblocks/blob/master/relaybox/relays.jpg)  
