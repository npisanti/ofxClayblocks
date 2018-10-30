
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include <atomic>
#include "ofThread.h"
#include <mutex>

namespace ofx { namespace clayblocks {

class RPiezos : public ofThread {

public:
	~RPiezos();

    void start( int port, std::string oscDevice="/piezos" );
    void stop();

    bool trigger( int index, float threshold=0.0f );
	float envelope( int index );

    void flush();


	void drawEnvelopes( int x, int y, int w );
    void logTriggers();

private:
    std::string deviceName;
    std::string trigAddress;
    std::string envAddress;

	std::atomic<float> triggers[6];
	std::atomic<float> envelopes[6];

	ofxOscReceiver oscReceiver;
    void threadedFunction();

};

}}
