`cb_rpiezos`
==============

This is a system composed by a shield to mount on the rPi, a server application running on the rPi to elaborate signals and a client side calibration utility.

You need oF 0.10, and on the rPi you also need [ofxGPIO](https://github.com/kashimAstro/ofxGPIO).

The `cb_piezos` app has to be running on the raspberry pi. You have to edit the `bin/data/settings.json` file and set the IP number of your desktop for enabling the communication.

On client you need to give the IP number of your device to `ofx::clayblocks::Piezos::setup( std::string serverIP )`.

Each piezos has some parameters to change its settings:
- `active` : enables or disables OSC messages for this sensor
- `threshold low` : reading lower than this are not sent to the client
- `threshold high` : reading higher than this are not sent to the client
- `attack` : value for smoothing the signal attack, higher is for a slower envelope
- `release` : value for smoothing the signal release, higher is for a slower envelope
- `trigger threshold` : threshold to surpass for getting a trigger 
- `retrig threshold` : no more trigger are intercepted before the signal goes down below this threshold

You can access to this parameters by code (or GUI) from the `ofx::clayblocks::Piezos` class, it has a public `piezos` vector for the 6 piezos and each item in the vector exposes all those parameter with the corresponding name written in camel case. Run the `example_piezos` for a practical explanation.

For more info on how to build the hardware, go into the [hardware](https://github.com/npisanti/ofxClayblocks/tree/master/cb_rpiezos/hardware) folder.

perfboard prototype:
<p align="center">
  <img src="https://raw.githubusercontent.com/npisanti/ofxClayblocks/master/cb_rpiezos/prototype.jpg" width="700">
</p>
