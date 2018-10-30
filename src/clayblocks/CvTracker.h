
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

namespace ofx { namespace clayblocks {

class CvTracker{

public:
    CvTracker();

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

    ofParameterGroup tracker;
        ofParameter<bool> doBackgroundSubtraction;
        ofParameter<bool> denoise;
        ofParameter<int> low;
        ofParameter<int> high;
        ofParameter<int> minArea;
        ofParameter<int> maxArea;
        ofParameter<int> distSensitivity;
        ofParameter<float> veloSensitivity;
        ofParameter<int> persistence;
        ofParameter<int> maxDistance;
        ofParameter<float> minX, maxX, minY, maxY;
        ofParameter<bool> sendContours;
        ofParameter<float> simplifyContours;
        ofParameter<int> sendImage;

    ofParameterGroup simulation;
        ofParameter<bool> simulate;
        ofParameter<int> simulatedBlobs;
        ofParameter<float> speed;

    ofParameterGroup mapping;
        ofParameter<bool> active;
        ofParameterGroup input;
        ofParameter<float> topX0;
        ofParameter<float> topX1;
        ofParameter<float> topY; 
        ofParameter<float> bottomX0;
        ofParameter<float> bottomX1;
        ofParameter<float> bottomY;
        ofParameterGroup output;
        ofParameter<int> outX;
        ofParameter<int> outY;
        ofParameter<int> outW;
        ofParameter<int> outH;

    void mapToSurface( int w, int h );

    void startRecordingAverage();
    void stopRecordingAverage();

private:
    void updateBlob( Blob & blob, ofxOscMessage & m );
    void onSimulate( bool & value );
    

    void doMapping( float & x, float & y );

    ofxOscReceiver oscReceiver;
    ofxOscParameterSync sync;

    bool bImageReceived;
    ofImage receivedImage;

    float t0;

    bool bRecordAverage;
    glm::vec2 average;
    std::vector<glm::vec2> recorded;

};

}}
