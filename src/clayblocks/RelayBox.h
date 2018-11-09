
#pragma once 

#include "ofMain.h"
#include "ofxOsc.h"

namespace ofx{ namespace clayblocks {
    
class RelayBox {
    
public:
    void setup( std::string serverIP );

    ofParameterGroup parameters;
        std::vector<ofParameter<bool>> relays;
        
    ofParameterGroup & label( std::string deviceName );
    void label( int index, std::string relayLabel );

private:
    ofxOscSender sender;

    std::vector<std::string> addresses;
    
    template<int index> void onBoolChange( bool & value );

};
      
}}
