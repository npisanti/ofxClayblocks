
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

namespace ofx { namespace clayblocks {

class CvTracker{

public:

    struct Blob {
        Blob();

        unsigned int label;
        glm::vec2 position;
        glm::vec2 velocity;
        ofRectangle boundaries;
        ofPolyline contour;
    };

    void setup( std::string serverIP );

    void update();

    void draw( int x=0, int y=0, int w = 640, int h=480 );

    ofParameterGroup & label( std::string name );

    std::vector<Blob> blobs;

    ofParameterGroup parameters;

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

    ofParameterGroup simulation;
        ofParameter<bool> simulate;
        ofParameter<int> simulatedBlobs;
        ofParameter<float> speed;

    ofParameterGroup sync;
    
protected: 
    virtual void cameraOptions() = 0;
    
    std::string classname;


private:
    void updateBlob( Blob & blob, ofxOscMessage & m );
    void onSimulate( bool & value );

    ofxOscReceiver oscReceiver;
    ofxOscParameterSync synchronizer;

    bool bImageReceived;
    ofImage receivedImage;

    float t0;

};

}}
