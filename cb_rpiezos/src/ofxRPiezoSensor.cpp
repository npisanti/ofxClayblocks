
#include "ofxRPiezoSensor.h"

#ifdef __ARM_ARCH

ofxRPiezoSensor::ofxRPiezoSensor() {
    oldValue = 0;
    counter = 1;    
    rising = true;
}

void ofxRPiezoSensor::setup ( int channel, MCP & mcp, ofxOscSender & sender, ofxOscSender & calib, string name ) {
    this->channel = channel;
    this->a2d = &mcp;
    this->sender = &sender;
    this->calib = &calib;
    envAddress.resize(2);
    trigAddress.resize(2);
    
    rawAddress = "/" + name + "/raw";
    envAddress = "/" + name + "/envelope";
    trigAddress = "/" + name + "/trigger";
}

void ofxRPiezoSensor::update( bool bCalibrate ) {
    
    if( bActive ){
        unsigned char data[3];
        data[0] = 1;
        data[1] = 0b10000000 |( ((channel & 7) << 4));
        data[2] = 0;
        a2d->readWrite(data);//sizeof(data) );
        int value = 0;
        value = (data[1]<< 8) & 0b1100000000;
        value |=  (data[2] & 0xff);     
        // now 'value' holds the ADC in value

        if(value!=oldValue){
            
            if(bCalibrate){
                ofxOscMessage m;
                m.setAddress( rawAddress );
                m.addIntArg( channel );
                m.addIntArg( value );
                calib->sendMessage(m, false);
            }

            bool risingNow = ( value > oldValue );
            
            if ( bTrigger && rising && !risingNow && counter>0 && (value > gateLo) ){ 
                // if all those things were true we are on the fall of a peak
                float dyn = ofMap( value, gateLo, gateHi, 0.0f, 1.0f, true );
                ofxOscMessage m;
                m.addIntArg( channel );
                m.setAddress( trigAddress );
                m.addFloatArg( dyn );
                if(bCalibrate){ 
                    calib->sendMessage(m, false ); 
                } else { 
                    sender->sendMessage(m, false );
                }
                counter = -retrigger; // to avoid retriggering      
            }
            
            rising = risingNow;
            oldValue = value;
            
            if( bEnvelope ) xn = ofMap( value, envLo, envHi, 0.0f, 1.0f, true );
        }
        counter++;

        // envelope follower
        if( bEnvelope ) {
            float yn_m1 = yn;
            
            if (xn > yn ) {
                yn = attack  * (yn - xn) + xn;
            } else {
                yn = release * (yn - xn) + xn;
            } 
            
            if( abs( yn_m1 - yn ) > granularity ){
                ofxOscMessage m;
                m.setAddress( envAddress );
                m.addIntArg( channel );
                m.addFloatArg( yn );
                if(bCalibrate){ 
                    calib->sendMessage(m, false ); 
                } else { 
                    sender->sendMessage(m, false );
                }
            }
            
            if( yn!=0.0f && yn < granularity ) { // rounds to zero
                yn = 0.0f;

                ofxOscMessage m;
                m.setAddress( envAddress );
                m.addIntArg( channel );
                m.addFloatArg( yn );
                if(bCalibrate){ 
                    calib->sendMessage(m, false ); 
                } else { 
                    sender->sendMessage(m, false );
                }
            }
            
        }
    }
    
}

#endif
