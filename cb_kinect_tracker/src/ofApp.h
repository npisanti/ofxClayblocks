#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxOscParameterSync.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "OSCTracking.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();

    ofxKinect kinect;
    ofxCvGrayscaleImage gray;
    
    int width, height;
    
    cv::Mat frame;
    
    ofParameterGroup settings;
    
        ofParameterGroup camera;
            ofParameter<int> tilt;
            ofParameter<bool> useDepth;
        
        ofParameterGroup network;
            ofParameter<std::string> ip;
            ofParameterGroup ports;
            ofParameter<int> trackingPort;
            ofParameter<int> imagePort;
            ofParameter<int> syncSendPort;
            ofParameter<int> syncReceivePort;

        ofParameter<bool> drawToScreen;
        
    ofx::helper::OSCTracking tracking;
    
    void onTilt( int & value );
    void onUseDepth( bool & value );
    
    bool currentMode;
};
