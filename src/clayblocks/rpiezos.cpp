
#include "rpiezos.h"


void ofx::clayblocks::rpiezos::start( int port, std::string oscDevice ){

    deviceName = oscDevice;
    if(deviceName.substr(0, 1)!="/"){
        deviceName = "/" + deviceName;
    }

    trigAddress = deviceName + "/trigger";
    envAddress = deviceName + "/envelope";

    std::cout << "[ofx::clayblocks::rpiezos] listening for piezo OSC messages on port "<<port<<"\n";
	oscReceiver.setup( port );

	ofSleepMillis(50);
	startThread();

	for(int i=0; i<6; ++i){
		envelopes[i] = 0.0f;
	}
	flush();
}

void ofx::clayblocks::rpiezos::stop(){
	stopThread();
}

ofx::clayblocks::rpiezos::~rpiezos(){
	if(isThreadRunning()){
        stopThread();
    }
    ofSleepMillis(50);
}



void ofx::clayblocks::rpiezos::threadedFunction() {

	while(isThreadRunning()) {
		while(oscReceiver.hasWaitingMessages()){
			// get the next message
			ofxOscMessage m;
			oscReceiver.getNextMessage(m);

			if(m.getAddress() == trigAddress ){
				int i =  m.getArgAsInt32(0);
				float t = m.getArgAsFloat(1);
				if(i>=0 && i<6) triggers[i] = t;
			}

			else if(m.getAddress() == envAddress ){
				int i =  m.getArgAsInt32(0);
				float e = m.getArgAsFloat(1);
				if(i>=0 && i<6) envelopes[i] = e;
			}
		}

		ofSleepMillis(5);

	}
}



bool ofx::clayblocks::rpiezos::trigger( int index, float threshold ){
    if( triggers[index] > threshold ){
        triggers[index] = -1.0f;
        return true;
    }
    return false;
}

float ofx::clayblocks::rpiezos::envelope( int index ){
    return envelopes[index].load();
}

void ofx::clayblocks::rpiezos::flush() {
	for(int i=0; i<6; ++i){
		triggers[i] = -1.0f;
	}
}



void ofx::clayblocks::rpiezos::drawEnvelopes( int x, int y, int w ) {
	ofPushMatrix();
	ofTranslate( x, y + 10 );
		const int h = 14;
		const int sep = 25;
		ofFill();
        for (int i=0; i<6; ++i ){
            ofDrawBitmapString( "piezo "+ofToString(i), 0, sep*i +11 );
            ofDrawRectangle( 40, sep*i, envelopes[i]*(w-40), h );
        }
	ofPopMatrix();
}

void ofx::clayblocks::rpiezos::logTriggers(){
    for( int i=0; i<6; ++i){
        if( trigger(i)){
            std::cout<<"[ofx::clayblocks::rpiezos] catched trigger on piezo "<<i<<"\n";
        }
    }
}
