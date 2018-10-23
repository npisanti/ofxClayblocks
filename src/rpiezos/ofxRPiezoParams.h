
#pragma once

#include "ofMain.h"

class ofxRPiezoParams {
    
public:
    ofxRPiezoParams();

    ofParameterGroup parameters;        
        ofParameter<bool>   bActive;
        
        ofParameter<bool>   bTrigger;
        ofParameter<int>    gateLo;
        ofParameter<int>    gateHi;
        ofParameter<int>    retrigger; 
        
        ofParameter<bool>   bEnvelope;
        ofParameter<float>  attack;
        ofParameter<float>  release;
        ofParameter<int>    envHi;
        ofParameter<int>    envLo;
        ofParameter<int>    precision; 

protected:
    float granularity;
    
private:
    void precisionChanged( int & value );
    static int number;

};
