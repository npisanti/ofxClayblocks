`cb_ir_tracker`
==============

This is a system composed by a pimoroni fisheye camera without infrared, a server application running on the rPi to elaborate signals and a client side class.

For notes about the camera to use, go to [hardware](https://github.com/npisanti/ofxClayblocks/tree/master/cb_ir_tracker/hardware).

For my notes about getting the right infrared light, go to [infrared](https://github.com/npisanti/ofxClayblocks/tree/master/cb_ir_tracker/infrared).

For compiling the server app you need oF 0.10, [ofxCv](https://github.com/kylemcdonald/ofxCv.git) and [ofxCvPiCam](https://github.com/orgicus/ofxCvPiCam.git). You need to remove the `libs/mmal` folder of the latter.
```sh
    cd openFrameworks
    cd addons 
    git clone https://github.com/kylemcdonald/ofxCv.git
    git clone https://github.com/orgicus/ofxCvPiCam.git
    rm -rf ofxCvPiCam/libs/mmal
```

The `cb_ir_tracker` app has to be running on the raspberry pi. You have to edit the `bin/data/settings.json` file and set the IP number of your desktop for enabling the communication.

On client you need to give the IP number of your device to `ofx::clayblocks::IRTracker::setup( std::string serverIP )`.

The tracker has some parameters to change its settings:
- `background subtraction` : activates background subtraction
- `take background` : click to retake the background again
- `denoise` : activate a median blur for denoising, quite cpu-expensive
- `threshold low` : low pixel value for cv threshold
- `threshold high` : high pixel value for cv threshold
- `area min` : minimum area for tracking, exp cubic control from zero to 1/16th of the total area
- `area max` : maximum area for tracking, exp cubic control from zero the total area
- `persistence' : determines how many frames an object can last without being seen until the tracker forgets about it
- `max distance` : how far an object can move until the tracker considers it a new object
- `filter delta distance` : don't send an OSC message if the blob doesn't move more than this distance
- `filter delta velocity` : don't send an OSC message if the blob doesn't have a change in speed bigger that this value
- `filter min X` : don't send an OSC message based on center position, filters low x boundary
- `filter max X` : don't send an OSC message based on center position, filters high x boundary
- `filter min Y` : don't send an OSC message based on center position, filters low y boundary
- `filter max Y` : don't send an OSC message based on center position, filters high y boundary
- `send contours` : send full contour information
- `simplify contours` : amount of contour simplification before sending OSC
- `send image' : when it's different than 0 a low res version of the camera image is sent over OSC, enable it just for setting up and then deactivate it to save network bandwidth, 0 is off, 1 is the camera image, 2 the thresholded image

You can access to this parameters by code (or GUI) from the `ofx::clayblocks::IRTracker` class. the member names for the class are the corresponding name written in camel case. The class has also other parameters to change the camera settings and enable a simulation of the blobs when you want to try something without the rpi device.

The `example_trackers` is an example/demo for the clients of bot this class and `ofx::clayblocks::KinectTracker`, select the one you want to use in `ofApp.h`.
