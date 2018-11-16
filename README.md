ofxClayblocks
==============
`ofxClayblocks` is a collection of devices that send and receive OSC messages over the network. Most of them are raspberry pi based and convert sensors reading to OSC messages or control actuators by receiving OSC messages.

The server apps are:
- `cb_accelerometer` runs on your android phones and send accelerometer data
- `cb_capacitive` : Adafruit MPR121 capacitive sensing
- `cb_ir_tracker` : cv blobs detection with pimoroni fisheye IR camera
- `cb_kinect_tracker` : cv blobs detection with the kinect
- `cb_relaybox` : controls relay modules
- `cb_piezos` : piezos sensors device

Relative openFrameworks receiver classes to have a fast to use client side API are also included. 

Go into the respective server app folder for a specific README.md with more informations about usage, pictures and code building instructions.

Nicola Pisanti, MIT License, 2018
