
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

namespace ofx { namespace clayblocks {

class cvtracker{

public:
    cvtracker();

    struct Blob {
        Blob();

        unsigned int label;
        glm::vec2 position;
        glm::vec2 velocity;
        ofRectangle boundaries;
        ofPolyline contour;
    };

    void setup( int oscPort, std::string serverIp, int syncReceivePort=4243, int syncSendPort=4244 );

    void update();

    void draw( int x=0, int y=0, int w = 640, int h=480 );

    std::vector<Blob> blobs;

    ofParameterGroup parameters;
        ofParameter<bool> doBackgroundSubtraction;
        ofParameter<bool> denoise;
        ofParameter<int> low;
        ofParameter<int> high;
        ofParameter<int> minArea;
        ofParameter<int> maxArea;
        ofParameter<float> distSensitivity;
        ofParameter<float> veloSensitivity;
        ofParameter<int> persistence;
        ofParameter<int> maxDistance;
        ofParameter<float> minX, maxX, minY, maxY;
        ofParameter<bool> remap;
        ofParameter<bool> sendContours;
        ofParameter<float> simplifyContours;
        ofParameter<int> sendImage;

    ofParameterGroup simulation;
        ofParameter<bool> simulate;
        ofParameter<int> simulatedBlobs;
        ofParameter<float> speed;

private:
    void updateBlob( Blob & blob, ofxOscMessage & m );

    ofxOscReceiver oscReceiver;
    ofxOscParameterSync sync;

    bool bImageReceived;
    ofImage receivedImage;

    float t0;

};

}}
