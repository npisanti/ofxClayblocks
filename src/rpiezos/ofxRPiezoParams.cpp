
#include "ofxRPiezoParams.h"

int ofxRPiezoParams::number = 0;
    
ofxRPiezoParams::ofxRPiezoParams() {
    parameters.setName( "piezo " + ofToString(number++) );
    parameters.add( bActive.set("active", true ) );
    parameters.add( bTrigger.set("trigger output", true ) );
    parameters.add( gateLo.set("g low boundary", 120, 0, 1023 ) );
    parameters.add( gateHi.set("g high boundary", 1023, 0, 1023 ) );
    parameters.add( retrigger.set("retrigger limit", 60, 1, 500) );
    parameters.add( bEnvelope.set("env follower output", false ) );
    parameters.add( attack.set("attack fc", 0.8f, 0.75f, 0.9999f ) );
    parameters.add( release.set("release fc", 0.9f, 0.75f, 0.9999f ) );
    parameters.add( envLo.set("e low boundary", 10, 0, 1023 ) );
    parameters.add( envHi.set("e high boundary", 1023, 0, 1023 ) );
    precision.addListener( this, &ofxRPiezoParams::precisionChanged );
    parameters.add( precision.set("precision", 12, 6, 16 ) );
}

void ofxRPiezoParams::precisionChanged( int & value ) {
    granularity = 1.0f / powf(2.0f, precision);
}
