
#include "cvtracker/CvTracker.h"


namespace ofx { namespace clayblocks { 
    
class IRTracker : public CvTracker {

public:
    ofParameterGroup camera;
        ofParameter<int> saturation;
        ofParameter<int> sharpness;
        ofParameter<int> contrast;
        ofParameter<int> brightness;
        ofParameter<int> awbMode;
        ofParameter<int> exposureMode;
        ofParameter<bool> useRedChannel;

private:  
    void cameraOptions() override;
  
};
    
}}
