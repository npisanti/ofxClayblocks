
#include "Capacitive.h"

ofx::clayblocks::Capacitive::Capacitive(){
    // ----------- parameters setup -----------
    parameters.setName( "Capacitive" );
    parameters.add( touchThreshold.set("touch threshold", 12, 0, 255) );
    parameters.add( releaseThreshold.set("release threshold", 6, 0, 255) );
}

void ofx::clayblocks::Capacitive::setup( std::string serverIP ){
    
    // --------------- generating ports ------------------------------------ 
    int port = 12345;
    
    std::string portGenerator = serverIP;
    
    if( serverIP=="localhost" ){ // generate ports from first IP
        const auto & myIPs = ofSplitString( ofSystem( "hostname -I" ), " " );
        portGenerator = myIPs[0];
    }
    
    const auto & addressNumbers = ofSplitString( portGenerator, "." );
    port = 1000 + ofToInt(addressNumbers[3]);
    int serverSyncSend = 2000 + ofToInt(addressNumbers[3]);
    int serverSyncReceive = 3000 + ofToInt(addressNumbers[3]);

    // ---------------- OSC setup ----------------------------------------------
    receiver.setup( port ); 
    std::cout<<"[ofx::clayblocks::Capacitive] receiving OSC on port "<<port<<"\n";   

    synchronizer.setup( parameters, serverSyncSend, serverIP, serverSyncReceive );

    std::cout<<"[ofx::clayblocks::Capacitive] syncing parameters on port "<<serverSyncReceive<<" (send) and port "<<serverSyncSend<<" (receive)\n";
         
    // ------------ vectors setup ----------------------------------------------
    touches.resize( 12 );
    for( auto & value : touches ){ value = 0; }
    triggers.resize( touches.size() );
    for( auto & value : triggers ){ value = 0; }
}


void ofx::clayblocks::Capacitive::update(){
    synchronizer.update();
    
    for( auto & value : triggers ){ value = 0; }
    
	while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(m);

		if(m.getAddress() == "/capacitive"){
            int index = m.getArgAsInt32(0);
            int value = m.getArgAsInt32(1);
            if( index>=0 && index<touches.size() ){
                if( touches[index]==0 && value==1 ){
                    triggers[index] = 1;
                }
                touches[index] = value;
            }
        }
    }
}

bool ofx::clayblocks::Capacitive::trigger( int index ) const{
    if(index>=0 && index<12){
        return triggers[index];
    }else{
        std::cout<<"[ofx::clayblocks::Capacitive] wrong trigger index!\n";
        return false;
    }
}

bool ofx::clayblocks::Capacitive::touched( int index ) const{
    if(index>=0 && index<12){
        return touches[index];
    }else{
        std::cout<<"[ofx::clayblocks::Capacitive] wrong touch index!\n";
        return false;
    }
}
