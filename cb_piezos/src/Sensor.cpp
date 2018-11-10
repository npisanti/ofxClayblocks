
#include "Sensor.h"

ofx::clayblocks::piezo::Sensor::Sensor() {
    oldValue = 0;
    a2d = nullptr;
    sender = nullptr;
}

void ofx::clayblocks::piezo::Sensor::setup( int channel, MCP & mcp, ofxOscSender & sender, string oscAddress ){
    this->channel = channel;
    this->a2d = &mcp;
    this->sender = &sender;
    this->address = oscAddress;
}

void ofx::clayblocks::piezo::Sensor::update(){
#ifdef __ARM_ARCH
    if( active ){
        unsigned char data[3];
        data[0] = 1;
        data[1] = 0b10000000 |( ((channel & 7) << 4));
        data[2] = 0;
        a2d->readWrite(data);//sizeof(data) );
        int value = 0;
        value = (data[1]<< 8) & 0b1100000000;
        value |=  (data[2] & 0xff);     
        // now 'value' holds the ADC in value

        if( value!=oldValue && value>=thresholdLow && value<=thresholdHigh ){
            ofxOscMessage m;
            m.setAddress( address );
            m.addIntArg( channel );
            m.addIntArg( value );
            sender->sendMessage(m, false);
        }
    }
#endif
}
