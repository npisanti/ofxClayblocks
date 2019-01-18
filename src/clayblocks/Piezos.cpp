
#include "Piezos.h"


// ------------------------- PIEZO PROCESSOR -----------------------------------

void ofx::clayblocks::Piezos::PiezoProcessor::setup( int index ){
    
    parameters.setName( "sensor "+ofToString(index) );
    parameters.add( active.set("active", true ) );
    parameters.add( thresholdLow.set("threshold low", 0, 0, 1023 ) );
    parameters.add( thresholdHigh.set("threshold high", 1023, 0, 1023 ) );
    parameters.add( attack.set("attack", 0.1f, 0.0f, 0.9999f) );
    parameters.add( release.set("release", 0.7f, 0.0f, 0.9999f) );
    parameters.add( triggerThreshold.set("trigger threshold", 0.2f, 0.0f, 1.0f) );
    parameters.add( retrigThreshold.set("retrig threshold", 0.1f, 0.0f, 1.0f ) );

    syncgroup.setName( "sensor "+ofToString(index) );
    syncgroup.add( active );
    syncgroup.add( thresholdLow );
    syncgroup.add( thresholdHigh );  
    
    value = 0.0f;
    trig = false;
    env = 0.0f;
    gate = false;
    
    buffersize = 128;
    triggerDuration = 10;
        
    valueHistory.resize(buffersize);
    for( auto & value : valueHistory ){ value = 0.0f; }
    envHistory.resize( buffersize );
    for( auto & value : envHistory ){ value = 0.0f; }
    bufferIndex = 0;
    triggerCounter = triggerDuration + 1;
   
}

void ofx::clayblocks::Piezos::PiezoProcessor::update(){
    
    if ( value > env ) {
        env = env * attack + (1.0f - attack) * value; 
    } else {
        env = env * release + (1.0f - release) * value; 
        env = ( env < 0.0000001f ) ? 0.0f : env;
    } 
    
    trig = false;
    if( gate ){
        if( value <= retrigThreshold ){
            gate = false;
        }
    }else{
        if( value > triggerThreshold ){
            gate = true;
            trig = true;
        }
    }
    
}

void ofx::clayblocks::Piezos::PiezoProcessor::draw( float w, float h, const ofColor & color ){
    
    if( active ){
        valueHistory[bufferIndex] = value;
        envHistory[bufferIndex] = env;
        triggerCounter = trig ? 0 : triggerCounter;
        
        ofSetColor( color );
        float y = (1.0f-triggerThreshold) * h; 
        ofDrawLine( 0, y, w, y );   

        ofSetColor( color * 0.55f );
        y = (1.0f-retrigThreshold) * h; 
        ofDrawLine( 0, y, w, y );

        // --- draws raw data  ----
        ofSetColor( 120 );
        ofNoFill();
        ofBeginShape();
        for( float x=0; x<w; x+=1.0f ){
            int index = (x / w) * float(buffersize);
            index += bufferIndex;
            int indexoff = index - buffersize;
            index = ( index < buffersize ) ? index : indexoff;
            ofVertex( w-x, (1.0f-valueHistory[index])*h );
        }
        ofEndShape();

        // --- draws envelope plot and triggers ----
        ofBeginShape();
        if( triggerCounter<triggerDuration ){ 
            ofFill(); 
            ofVertex( w, h );
        }else{
            ofNoFill();
        }
        ofSetColor( color );
        
        for( int n=0; n<buffersize; ++n ){
            int index = n + bufferIndex;
            int indexoff = index - buffersize;
            index = ( index < buffersize ) ? index : indexoff;
            
            float x = (float(n) / float(buffersize)) * w;
            ofVertex( w-x, (1.0f-envHistory[index])*h );
        }
        
        if( triggerCounter<triggerDuration ){ 
            ofVertex( 0.0f, h );
            ofEndShape( true );
        }else{
            ofEndShape( );
        }

        // --- updates index and counters ----
        bufferIndex--;
        if( bufferIndex<0 ){
            bufferIndex = valueHistory.size()-1;
        }
        triggerCounter++;        
    
    }else{
    
        ofSetColor( color * 0.5f );
    
    }
     
    ofNoFill();        
    ofDrawRectangle( 0, 0, w, h );
    ofDrawBitmapString( parameters.getName(), 10, 18 );

}

// ------------------------- PIEZOS ----------------------------------------

void ofx::clayblocks::Piezos::setup( std::string serverIP ){
        
    // --------------- generate and add parameters -------------------------
    address = "/piezos";
    
    parameters.setName( "Piezos" );
    sync.setName( "sync piezos" );
    
    piezos.resize( 6 );
    for( size_t i=0; i<piezos.size(); ++i ){
        piezos[i].setup( i );
        parameters.add( piezos[i].parameters );
        sync.add( piezos[i].syncgroup );
    }

    // --------------- generating ports ------------------------------------ 
    int port = 12345;
    
    std::string portGenerator = serverIP;
    
    if( serverIP=="localhost" ){ // generate ports from first IP
        const auto & myIPs = ofSplitString( ofSystem( "hostname -I" ), " " );
        portGenerator = myIPs[0];
    }
    
    const auto & addressNumbers = ofSplitString( portGenerator, "." );
    port = 2000 + ofToInt(addressNumbers[3]);
    int serverSyncSend = 3000 + ofToInt(addressNumbers[3]);
    int serverSyncReceive = 4000 + ofToInt(addressNumbers[3]);

    // ---------------- OSC setup ----------------------------------------------
    receiver.setup( port ); 
    std::cout<<"[ofx::clayblocks::Piezos] receiving OSC on port "<<port<<"\n";   

    synchronizer.setup( sync, serverSyncSend, serverIP, serverSyncReceive );
    std::cout<<"[ofx::clayblocks::Piezos] syncing parameters on port "<<serverSyncReceive<<" (send) and port "<<serverSyncSend<<" (receive)\n";

}


void ofx::clayblocks::Piezos::update() {

    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage( m );

        if(m.getAddress() == address ){
            int index =  m.getArgAsInt32(0);
            float value = m.getArgAsInt32(1);
            if( index>=0 && index<6 ){
                piezos[index].value = ofMap( value, piezos[index].thresholdLow, piezos[index].thresholdHigh, 0.0f, 1.0f, true );
            }
        }
    }

    for( auto & piezo : piezos ){ piezo.update(); }
}

void ofx::clayblocks::Piezos::draw( float x, float y, float w, float h, const ofColor & color ){
    static const float separator = 20.0f;
    float piezoW = (w - separator) / 2.0f;
    float piezoH = (h - separator*2.0f) / 3.0f;
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate( x, y );
    ofPushMatrix();
    
        for( size_t i=0; i<piezos.size(); ++i ){
    
            piezos[i].draw( piezoW, piezoH, color );  
            ofTranslate( 0, piezoH + separator );    
            
            if( i==2 ){
                ofPopMatrix();
                ofTranslate( piezoW + separator, 0 );
                ofPushMatrix();
            }      
        }    
        
    ofPopMatrix();
    ofPopMatrix();
    ofPopStyle();
}

void ofx::clayblocks::Piezos::drawEnvelopes( int x, int y, int w, std::string name, int max ){
    ofDrawBitmapString( name, x, y + 21 );
    
    const int sep = 25;
    const int h = 14;
    
	ofPushMatrix();    
	ofTranslate( x, y + 10 + sep );
		ofFill();
        for (int i=0; i<max; ++i ){
            ofDrawBitmapString( "piezo "+ofToString(i), 0, sep*i +11 );
            ofDrawRectangle( 40, sep*i, piezos[i].value*(w-40), h );
        }
	ofPopMatrix();
}

ofParameterGroup & ofx::clayblocks::Piezos::label( std::string name ){
    parameters.setName( name );
    return parameters;
}
