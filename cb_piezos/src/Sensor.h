
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "piezo/Params.h"
#include "ofxGPIO.h"

namespace ofx{ namespace clayblocks{ namespace piezo{
    
class Sensor : public Params {
public:
    Sensor();
    
    void setup ( int channel, MCP & mcp, ofxOscSender & sender, string oscAddress="/piezos" );
    void update();

private:
    int channel;
    MCP* a2d;
    ofxOscSender * sender;
    
    int oldValue;
    
    string address;
};

}}}
