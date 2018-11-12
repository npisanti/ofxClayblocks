
#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

namespace np { namespace helper {

class OSCTracking {

public:
    OSCTracking();
    ~OSCTracking();

    void setup( int width, int height, const cv::Mat & toImitate );
    void update( cv::Mat & frame );
    void updateSync();

    ofParameterGroup tracker;
        ofParameter<bool> doBackgroundSubtraction;
        ofParameter<bool> takeBackground;
        ofParameter<bool> denoise;
        ofParameter<int> thresholdLow;
        ofParameter<int> thresholdHigh;
        ofParameter<int> minArea;
        ofParameter<int> maxArea;
        ofParameter<int> persistence;
        ofParameter<int> maxDistance;

        ofParameter<int> filterDeltaDistance;
        ofParameter<float> filterDeltaVelocity;
        ofParameter<float> filterRatio;
        ofParameter<float> filterMinX, filterMaxX, filterMinY, filterMaxY;

        ofParameter<bool> sendContours;
        ofParameter<float> simplifyContours;
        ofParameter<int> sendImage;


    ofParameterGroup network;
        ofParameter<std::string> clientIP;
        

    ofParameterGroup sync;

    cv::Mat tLow;
    cv::Mat tHigh;
    cv::Mat thresh;

    ofxCv::ContourFinder finder;

private:
    // -------------------- internal class --------------------------
    class SensedBlob {
    public:
        SensedBlob(){
            label = 0;
            center.x = -1.0f;
            center.y = -1.0f;
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            boundaries = ofRectangle( 0, 0, 0, 0 );
            found = true;
            update = true;
        }

        unsigned int label;
        glm::vec2 center;
        glm::vec2 velocity;
        ofRectangle boundaries;
        ofPolyline contour;
        bool found;
        bool update;
    };

    // -------------------- members and functions -------------------

    int width, height;

    std::vector<SensedBlob> blobs;
    float divideW, divideH;

    ofxOscParameterSync synchronizer;
    ofxOscSender  sender;
    char * buffer;
    size_t buffersize;

    cv::Mat background;

    void doBlobs();

};


}}
