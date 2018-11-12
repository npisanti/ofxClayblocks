
## notes on infrared lights setup

The IR light produced by the included LEDs is not that strong and could illuminate just little spaces. For a bigger stage you will probably need to flood the area with stronger lights. When you shop for lights buy the one with big diffused leds like this:
<p align="center">
  <img src="https://raw.githubusercontent.com/npisanti/ofxClayblocks/master/cb_ir_tracker/infrared/light.jpg" width="700">
</p>
(this one works very well up to about 8m)

You should aim for a big contrast between the background and the object you want to track, so the objects should be IR reflective and the background not that much. 

Much of the color used to dye clothes are IR reflective (even the black color), but anyway you should check and test as much things you can. The most reflecting thing you can put on the object to track is [reflective safety tape](https://www.google.com/search?q=reflective+safety+tape&client=firefox-b-ab&source=lnms&tbm=isch&sa=X&ved=0ahUKEwjFl63g48_eAhWsJsAKHVFuDxEQ_AUIEigB&biw=1280&bih=618), but it works well only when the lights comes from the same direction of the camera.

If the background is too much IR-reflecting it could be a big problem, you should try to illuminate the object from the side, probably you will need at least 4 lights and it will still be difficult to not illuminate the background. Some background subtraction will also be needed if some of the lights are lens-flaring in the camera.
