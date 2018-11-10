
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGPIO.h"

namespace ofx{ namespace clayblocks{ namespace piezo{
    
class Sensor {
public:
    Sensor();
    
    void setup ( int channel, MCP & mcp, ofxOscSender & sender, string oscAddress="/piezos" );
    void update();

    ofParameterGroup parameters;        
        ofParameter<bool>   active;
        ofParameter<int>    thresholdLow;
        ofParameter<int>    thresholdHigh;

    ofParameterGroup & label( std::string name );

private:
    int channel;
    MCP* a2d;
    ofxOscSender * sender;
    
    int oldValue;
    
    string address;
};

}}}
