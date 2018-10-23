ofxClayblocks
==============
`ofxClayblocks` is a collection of devices that send and receive OSC messages over the network. Most of them are raspberry pi based and convert sensors reading to OSC messages or control actuators by receiving OSC messages.

It includes:
- `cb_accelerometer` runs on your android phones and send accelerometer data
- `cb_capacitive` : Adafruit MPR121 capacitive sensing
- `cb_irtracker` : blob detection with pimoroni fisheye IR camera
- `cb_kinectracker` : blob detection with the kinect
- `cb_relaybox` : controls relay modules
- `cb_rpiezos` : piezos sensors device

And relative openFrameworks receiver classes to have a fast to use client side API. The `ofx::clayblocks::cvtracker` class deals both with the IR and kinect trackers.

Go into the individual folders for usage, pictures and building instructions.

For the moment the android accelerometer server is written in [processing](https://processing.org/) with the [oscP5](http://www.sojamo.de/libraries/oscP5/) and the [ketai](http://ketai.org/) library.

The raspberry pi sketches are openFrameworks apps, so you need [openFrameworks](https://openframeworks.cc/), and also [ofxGPIO](https://github.com/kashimAstro/ofxGPIO) and ofxOsc (included in oF).