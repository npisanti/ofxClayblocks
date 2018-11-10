#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "Sensor.h"
#include "piezo/Constants.h"
#include "ofxGPIO.h"
#include "ofxOscParameterSync.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
        
        ofParameterGroup sync;
        ofParameterGroup parameters;
        ofParameterGroup network;
        
        vector<ofx::clayblocks::piezo::Sensor> piezos;
            
        ofParameter<std::string> clientIP;

        ofxOscSender sender;
        ofxOscParameterSync synchronizer;    
#ifdef __ARM_ARCH   
        MCP a2d;
#endif
};
