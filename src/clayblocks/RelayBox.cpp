
#include "RelayBox.h"

void ofx::clayblocks::RelayBox::setup( std::string serverIP ){
    
    // ------------ automatically generates port -------------------------------
    const auto & addressNumbers = ofSplitString( serverIP, "." );
    int port = 12345;
    
    if( addressNumbers[0]=="localhost" ){
        const auto & myIPs = ofSplitString( ofSystem( "hostname -I" ), " " );
        const auto & first = ofSplitString( myIPs[0], "." );
        port = 2000 + ofToInt( first[3] );
        std::cout<<"[ofx::clayblocks::RelayBox] sending messages to localhost, port "<<port<<"\n";
        sender.setup( serverIP, port ); 
    }else if( addressNumbers.size()  == 4 ){
        port = 2000 + ofToInt( addressNumbers[3] );
        std::cout<<"[ofx::clayblocks::RelayBox] sending messages to "<<serverIP<<", port "<<port<<"\n";
        sender.setup( serverIP, port ); 
    }else{
        ofLogError()<<"[ofx::clayblocks::RelayBox] malformed server IP";   
    }
    
    // ------------------ parameters setup -------------------------------------
    parameters.setName( "RelayBox" );

    relays.resize( 8 );
    addresses.resize( relays.size() );
    for( size_t i=0; i<addresses.size(); ++i ){
        addresses[i] = "/relay/k" + ofToString( i+1 );
    }

    relays[0].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<0> );
    relays[1].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<1> );
    relays[2].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<2> );
    relays[3].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<3> );
    relays[4].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<4> );
    relays[5].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<5> );
    relays[6].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<6> );
    relays[7].addListener( this, &ofx::clayblocks::RelayBox::onBoolChange<7> );

    for( size_t i=0; i<relays.size(); ++i ){
        parameters.add( relays[i].set( "K" + ofToString( i+1 ), false ) );
    }
}

template<int index> void ofx::clayblocks::RelayBox::onBoolChange( bool & value ){
    ofxOscMessage m;
    m.setAddress( addresses[index] );
    if( relays[index] ){
        m.addIntArg(1);
    }else{
        m.addIntArg(0);
    }
    sender.sendMessage(m, false);
}

ofParameterGroup & ofx::clayblocks::RelayBox::label( std::string deviceName ){
    parameters.setName( deviceName );
    return parameters;
}

void ofx::clayblocks::RelayBox::label( int index, std::string relayLabel ){
    relays[index].setName( "K"+ofToString(index+1) + " (" + relayLabel+")" );
}
