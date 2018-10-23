
#include "ofxRPiezosServer.h"

#ifdef __ARM_ARCH

void ofxRPiezosServer::setup( string xmlSettingsPath ){
    
    ofxXmlSettings settings;
    settings.loadFile( xmlSettingsPath );
    string ip = settings.getValue("settings:client_ip", "localhost");
    int port = settings.getValue("settings:port", 4444);   
    string name = settings.getValue("settings:name", "piezos");   
    
    setup( ip, port, name );
}

void ofxRPiezosServer::setup( string clientIP, int port, string name ){

    gui.setup("", "settings.xml", 20, 20);
    gui.setName( name );
    gui.add( bCalibrate.set("calibrate piezos", false ) );
    
    piezos.resize( OFXRPIEZOS_NUM_SENSORS );
    for( size_t i=0; i<piezos.size(); ++i ){
        gui.add( piezos[i].parameters );
    }
    
    gui.add( saveOnServer.set("save server settings", false ) );
    gui.add( loadFromServer.set("load server settings", false ) );
    
    saveOnServer.addListener( this, &ofxRPiezosServer::saveCallback );
    
    sender.setup( clientIP, port );
    calib.setup( clientIP, OFXRPIEZOS_DEBUG_PORT );
    a2d.setup("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
    
    for( int i=0; i<(int)piezos.size(); ++i ){
        piezos[i].setup( i, a2d, sender, calib, name );
    }
    
    sync.setup((ofParameterGroup&)gui.getParameter(), OFXRPIEZOS_SYNC_PORT_SERVER, clientIP, OFXRPIEZOS_SYNC_PORT_CLIENT );

    ofFile file("settings.xml");
	if ( file.exists() ) gui.loadFromFile("settings.xml");
    
    bCalibrate = false; // never start in calibration mode
}

void ofxRPiezosServer::update(){
    
    if(loadFromServer){ // works only this way, won't work with listeners (strange)
        ofFile file("settings.xml");
        if ( file.exists() ) { 
            gui.loadFromFile("settings.xml");
            cout<<"[rPiezos] settings retrieved\n";
        }else{
            cout<<"[rPiezos] no settings file found\n";
        }
        loadFromServer = false;
        bCalibrate = true;
    }
    
    sync.update();
    
    for( size_t i=0; i<piezos.size(); ++i ){
        piezos[i].update( bCalibrate );
    }
    
    ofSleepMillis(1); // for stability
}

void ofxRPiezosServer::exit(){
    a2d.quit();
}

void ofxRPiezosServer::saveCallback( bool & value ){
    if( value ){
        saveOnServer = false;
        bCalibrate = false;
        gui.saveToFile("settings.xml");
        cout<<"[rPiezos] settings saved\n";
    }
}

#endif
