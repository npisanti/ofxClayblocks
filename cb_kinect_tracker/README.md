`cb_kinect_tracker`
==============

This is a system composed by Kinect v1 (tested mostly with model 1414), a server application running on the rPi to elaborate signals and a client side class.

For compiling the server app you need oF 0.10 and [ofxCv](https://github.com/kylemcdonald/ofxCv.git).

The `cb_kinect_tracker` app has to be running on the raspberry pi (or on another desktop). You have to edit the `bin/data/settings.json` file and set the IP number of your desktop for enabling the communication.

On client you need to give the IP number of your device to `ofx::clayblocks::KinectTracker::setup( std::string serverIP )`.

The tracker has some parameters to change its settings:
- `background subtraction` : activates background subtraction
- `take background` : click to retake the background again
- `denoise` : activate a median blur for denoising, quite cpu-expensive
- `threshold low` : low pixel value for cv threshold
- `threshold high` : high pixel value for cv threshold
- `area min` : minimum area for tracking, exp cubic control from zero to 1/16th of the total area
- `area max` : maximum area for tracking, exp cubic control from zero the total area
- `persistence` : determines how many frames an object can last without being seen until the tracker forgets about it
- `max distance` : how far an object can move until the tracker considers it a new object
- `filter delta distance` : don't send an OSC message if the blob doesn't move more than this distance
- `filter delta velocity` : don't send an OSC message if the blob doesn't have a change in speed bigger that this value
- `filter min X` : don't send an OSC message based on center position, filters low x boundary
- `filter max X` : don't send an OSC message based on center position, filters high x boundary
- `filter min Y` : don't send an OSC message based on center position, filters low y boundary
- `filter max Y` : don't send an OSC message based on center position, filters high y boundary
- 'filter ratio' : don't send an OSC message one of the dimensions is too much bigger than the other
- `send contours` : send full contour information
- `simplify contours` : amount of contour simplification before sending OSC
- `send image` : when it's different than 0 a low res version of the camera image is sent over OSC, enable it just for setting up and then deactivate it to save network bandwidth, 0 is off, 1 is the camera image, 2 the thresholded image

You can access to this parameters by code (or GUI) from the `ofx::clayblocks::KinectTracker` class. the member names for the class are the corresponding name written in camel case. The class has also other parameters to change the camera settings and enable a simulation of the blobs when you want to try something without the rpi device.

The `example_trackers` is an example/demo for the clients of both `ofx::clayblocks::IRTracker` and `ofx::clayblocks::KinectTracker`, select the one you want to use in `ofApp.h`.
