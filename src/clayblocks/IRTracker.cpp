
#include "IRTracker.h"

void ofx::clayblocks::IRTracker::cameraOptions() {
    
    classname = "ofx::clayblocks::IRTracker";
    
    parameters.setName("IRTracker");
    
    sync.add( saturation.set("saturation",0,-100,100) );
    sync.add( sharpness.set("sharpness",0, -100,100) );
    sync.add( contrast.set("contrast", 50 , -100,100) );
    sync.add( brightness.set("brightness", 60, 0,100) );
    sync.add( awbMode.set("white balance mode", 7, 0, 10) );
    sync.add( exposureMode.set("exposure mode", 2, 0, 13) );
    
    camera.setName( "camera");
        camera.add( saturation );
        camera.add( sharpness );
        camera.add( contrast );
        camera.add( brightness );
        camera.add( awbMode );
        camera.add( exposureMode );    
    parameters.add( camera );
 
}
