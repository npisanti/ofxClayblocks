cb_relaybox
==============

Device to control sunfounder 8 relays module. You need to run it as root to access the GPIOs.

usage 
```
    sudo ./bin/relaybox
```
the configuration in `bin/data/settings.json` is loaded.   

this is a list of the received osc messages and the pins to connect:
```
osc=/relay/k1 | gpio=17
osc=/relay/k2 | gpio=18
osc=/relay/k3 | gpio=22
osc=/relay/k4 | gpio=23
osc=/relay/k5 | gpio=24
osc=/relay/k6 | gpio=25
osc=/relay/k7 | gpio=12
osc=/relay/k8 | gpio=13

```
You can change this default behavior by editing the `settings.json` file. If you want to use this app with the `ofx::clayblocks::RelayBox` client don't change the addresses but just the gpio pins number, otherwise the client won't work anymore.

<p align="center">
  <img src="https://raw.githubusercontent.com/npisanti/ofxClayblocks/master/cb_relaybox/relays.jpg" width="700">
</p>
