#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
#include "OSCTracking.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

    
    ofxCvPiCam cam;
    
    cv::Mat red;
    std::vector <cv::Mat> channels;
    
    cv::Mat frame;
    cv::Mat undistorted;

    
    ofxCv::Calibration calibration;

    
    ofParameterGroup settings;
        ofParameterGroup camera;
            ofParameter<int> width;
            ofParameter<int> height;
            ofParameter<std::string> file;
            ofParameter<int> saturation;
            ofParameter<int> sharpness;
            ofParameter<int> contrast;
            ofParameter<int> brightness;
            ofParameter<int> awbMode;
            ofParameter<int> exposureMode;
            ofParameter<bool> useRedChannel;

        ofParameter<bool> drawToScreen;
    
    np::helper::OSCTracking tracking;
    
    
};
