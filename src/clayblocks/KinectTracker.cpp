
#include "KinectTracker.h"

void ofx::clayblocks::KinectTracker::cameraOptions() {
    
    classname = "ofx::clayblocks::KinectTracker";
    
    parameters.setName("KinectTracker");
    
    sync.add( tilt.set("tilt", 0, -30, 30) );
    sync.add( useDepth.set("use depth", true) );
    
    kinect.setName( "kinect ");
        kinect.add( tilt );
        kinect.add( useDepth );
    parameters.add( kinect );
 
}
