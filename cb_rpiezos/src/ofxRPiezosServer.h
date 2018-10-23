
#pragma once

#ifdef __ARM_ARCH

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxRPiezoSensor.h"
#include "ofxRPiezosConstants.h"
#include "ofxGPIO.h"
#include "ofxOscParameterSync.h"
#include "ofxXmlSettings.h"

class ofxRPiezosServer {

public:
    void setup( string xmlSettingsPath );
    void setup( string clientIP, int port, string name="piezos");
    void update();
    void exit();
    
    ofxPanel gui;
    
    vector<ofxRPiezoSensor> piezos;
    
    ofParameter<bool> bCalibrate;
    ofParameter<bool> saveOnServer;
    ofParameter<bool> loadFromServer;

private:

    void saveCallback( bool & value );
    
    ofxOscParameterSync sync;    

    // osc senders
    ofxOscSender calib;
    ofxOscSender sender;
    MCP a2d;
    
};

#endif
