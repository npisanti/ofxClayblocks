
#pragma once

#include "ofMain.h"

namespace ofx { namespace clayblocks { namespace piezo {

class Params {
    
public:
    Params();

    ofParameterGroup parameters;        
        ofParameter<bool>   active;
        ofParameter<int>    thresholdLow;
        ofParameter<int>    thresholdHigh;

    ofParameterGroup & label( std::string name );
};

}}}
