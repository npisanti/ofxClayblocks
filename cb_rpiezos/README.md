`cb_rpiezos`
==============

This is a system composed by a shield to mount on the rPi, a server application running on the rPi to elaborate signals and a client side calibration utility.

You need oF 0.10, and on the rPi you also need [ofxGPIO](https://github.com/kashimAstro/ofxGPIO).

The `cb_rpiezos` app has to be running on the raspberry pi. You have to edit the `bin/data/ip.xml` file to setup the port and the ip of your desktop for the calibration.
Then you have to run the `calibrate_rpiezos` app on the desktop. Set the IP of the raspberry pi in the `bin/data/ip.xml` file in the app folder. In this app you can enable and disable the single piezos and set up the thresholds for triggering. You can also enable and set up the envelope follower for each piezo.
If the destination for the OSC messages is different from your desktop you can now edit the `ip.xml` file again and then relaunch the `server` app, otherwise you should just start your client app and parse the OSC messages as you wish. For more info on how to build the hardware, go into the [hardware](https://github.com/npisanti/ofxClayblocks/tree/master/cb_rpiezos/hardware) folder.

perfboard prototype:
<p align="center">
  <img src="https://raw.githubusercontent.com/npisanti/ofxClayblocks/master/cb_rpiezos/prototype.jpg" width="700">
</p>

calibration utility:
<p align="center">
  <img src="https://raw.githubusercontent.com/npisanti/ofxClayblocks/tree/master/cb_rpiezos/calibrate_utility.png" width="700">
</p>
