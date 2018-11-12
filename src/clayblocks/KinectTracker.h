
#include "cvtracker/CvTracker.h"


namespace ofx { namespace clayblocks { 
    
class KinectTracker : public CvTracker {

public:
    ofParameterGroup kinect;
        ofParameter<int> tilt;
        ofParameter<bool> useDepth;

private:  
    void cameraOptions() override;
  
};
    
}}
