
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include <atomic>
#include "ofThread.h"
#include <mutex>

namespace ofx{ namespace clayblocks {

class Accelerometer : public ofThread {

public:
	Accelerometer();
	Accelerometer( const Accelerometer & other);
	Accelerometer( Accelerometer && other);
	Accelerometer& operator= (const Accelerometer& other);
	Accelerometer& operator= (Accelerometer&& other) noexcept;
	~Accelerometer();


	std::atomic<float> x;
	std::atomic<float> y;
	std::atomic<float> z;
	std::atomic<float> average;

    void start( int port, std::string oscDevice="/phone" );
    void stop();
	void draw( int x, int y );

    ofParameterGroup & label( std::string name );

    ofParameterGroup parameters;
        ofParameter<int> averageFrames;
        ofParameter<float> easing;
        ofParameter<float> xMin;
        ofParameter<float> xMax;
        ofParameter<float> yMin;
        ofParameter<float> yMax;
        ofParameter<float> zMin;
        ofParameter<float> zMax;

private:
    std::string deviceName;
    std::string xname;
    std::string yname;
    std::string zname;

    std::vector<glm::vec3> values;
	size_t vindex;

	float ax, ay, az;
    float ax_z1, ay_z1, az_z1;

	ofxOscReceiver oscReceiver;

	float lpc;

    void threadedFunction();
};

}}
