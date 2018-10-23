
#pragma once

#ifdef __ARM_ARCH

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxRPiezoParams.h"
#include "ofxGPIO.h"

class ofxRPiezoSensor : public ofxRPiezoParams {
public:
    ofxRPiezoSensor();
    
    void setup ( int channel, MCP & mcp, ofxOscSender & sender, ofxOscSender & calib, string name="piezos" );
    void update( bool bCalibrate );

private:
    int channel;
    MCP* a2d;
    ofxOscSender * sender;
    ofxOscSender * calib;
    
    int oldValue;
    int counter;
    
    bool rising;
    
    float xn;
    float yn;
    
    string rawAddress;
    string envAddress;
    string trigAddress;

};

#endif
