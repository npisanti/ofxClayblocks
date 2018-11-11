
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

#include <mutex>

namespace ofx { namespace clayblocks {

class Piezos {
    
public:
    class PiezoProcessor{
        friend class Piezos;
        
        public:
            ofParameterGroup parameters;   
                ofParameter<bool>   active;
                ofParameter<int>    thresholdLow;
                ofParameter<int>    thresholdHigh;
                
                ofParameter<float>  attack;
                ofParameter<float>  release;
                ofParameter<float> triggerThreshold;
                ofParameter<float> retrigThreshold;   
                
                void draw( float w, float h, const ofColor & color );
            
            bool trigger() const { return trig; };
            float envelope() const { return env; }; 
        private:            
            void setup( int index );
            void update();
            
            float value;
            bool trig;
            float env;
            bool gate;
                                
            ofParameterGroup syncgroup;       
            std::vector<float> valueHistory;
            std::vector<float> envHistory;
            int bufferIndex;
            int buffersize;
            int triggerCounter;
            int triggerDuration;

    };
    
    std::vector<PiezoProcessor> piezos;
    
public:
    void setup( std::string serverIP );
    void update();
    void draw( float x, float y, float w, float h, const ofColor & color=ofColor::white );

    ofParameterGroup & label( std::string name );
    
    bool trigger( int index ) const { return piezos[index].trig; }
	float envelope( int index ) const {  return piezos[index].env; }

	void drawEnvelopes( int x, int y, int w );

    ofParameterGroup parameters;

private:
    ofParameterGroup sync;

    std::string address;

	ofxOscReceiver receiver;
    ofxOscParameterSync synchronizer;
};

}}
