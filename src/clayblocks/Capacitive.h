
#pragma once 

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

namespace ofx{ namespace clayblocks {
    
class Capacitive {
    
public:
    Capacitive();

    void setup( std::string serverIP );

    void update();
    
    bool trigger( int index ) const;
    bool touched( int index ) const;

    ofParameterGroup parameters;
        ofParameter<int> touchThreshold;
        ofParameter<int> releaseThreshold;

private:
    ofxOscReceiver receiver;
    ofxOscParameterSync synchronizer;
    
    std::vector<int> touches;
    std::vector<int> triggers;
};
      
}}
