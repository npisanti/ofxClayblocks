#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(false);
    
    // ------------- settings --------------------------------------------------
    parameters.setName( "piezos" );
    sync.setName( "sync piezos" );

    network.setName("network");
        network.add( clientIP.set("client ip", "localhost") );
    parameters.add( network );
    
    piezos.resize( 6 );
    
    for( size_t i=0; i<piezos.size(); ++i ){
        parameters.add( piezos[i].label( "sensor "+ofToString(i) ) );
        sync.add( piezos[i].parameters );
    }
    
    ofJson json = ofLoadJson("settings.json");
    ofDeserialize( json, parameters );
    
    //ofJson json;
    //ofSerialize( json, parameters );
    //ofSavePrettyJson( "settings.json", json );
    
    
    // --------------- generating ports ------------------------------------ 
    int port = 12345;
    
    const auto & myIPs = ofSplitString( ofSystem( "hostname -I" ), " " );

    // check for similarity to the clientIP
    int chosen = 0;
    int greatestSim = 0;
    for( size_t i=0; i<myIPs.size(); ++i ){
        int min = (clientIP.get().size() < myIPs[i].size()) ? clientIP.get().size() : myIPs[i].size();
        int similarity = 0;
        for(int k=1; k<min; ++k){
            if( clientIP.get().substr(0, k) == myIPs[i].substr(0, k) ){
                similarity++;
            }
        }
        if( similarity > greatestSim ){
            greatestSim = similarity;
            chosen = i;
        }
    }
    
    // in case of localhost, the first will be chosen 
    std::string portGenerator = myIPs[chosen]; 
    
    const auto & addressNumbers = ofSplitString( portGenerator, "." );
    port = 2000 + ofToInt(addressNumbers[3]);
    int serverSyncSend = 3000 + ofToInt(addressNumbers[3]);
    int serverSyncReceive = 4000 + ofToInt(addressNumbers[3]);
    
    // ------------------ OSC setup --------------------------------------------
        
    sender.setup( clientIP, port );
    std::cout<<"[cb_rpiezos] sending OSC to "<<clientIP<<" on port "<<port<<"\n";   
       
    synchronizer.setup( sync, serverSyncReceive, clientIP, serverSyncSend );
    std::cout<<"[cb_rpiezos] syncing parameters on port "<<serverSyncSend<<" (send) and port "<<serverSyncReceive<<" (receive)\n";

    // ------------------ inputs setup -----------------------------------------
#ifdef __ARM_ARCH        
    a2d.setup("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);

    for( int i=0; i<(int)piezos.size(); ++i ){
        piezos[i].setup( i, a2d, sender, "/piezos" );
    }
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
    
    synchronizer.update();

    for( size_t i=0; i<piezos.size(); ++i ){
        piezos[i].update();
    }
    
    ofSleepMillis(1); // for stability
}
