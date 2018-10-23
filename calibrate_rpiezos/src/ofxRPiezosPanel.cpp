
#include "ofxRPiezosPanel.h"

void ofxRPiezosPanel::setupFromFile( string xmlSettingsPath ) {
    ofxXmlSettings settings;
    settings.loadFile( xmlSettingsPath );
    string ip = settings.getValue("settings:server_ip", "localhost");
    string name = settings.getValue("settings:name", "piezos");
    int plot = settings.getValue("settings:max_plot", 127);
    setup( ip, name, plot );
}

void ofxRPiezosPanel::setup( string serverIP, string name, int maxplot ){

    rawAddress = "/" + name + "/raw";
    envAddress = "/" + name + "/envelope";
    trigAddress = "/" + name + "/trigger";

	string filename = name + ".xml";
    gui.setup("", filename, 20, 20);
    gui.setName( name );
    gui.add( bCalibrate.set("calibrate piezos", false ) );
    
    piezos.resize( OFXRPIEZOS_NUM_SENSORS );
    for( size_t i=0; i<piezos.size(); ++i ){
        gui.add( piezos[i].parameters );
    }
    
    gui.add( saveOnServer.set("save server settings", false ) );
    gui.add( loadFromServer.set("load server settings", false ) );
    gui.minimizeAll();
    
    saveOnServer.addListener ( this, &ofxRPiezosPanel::saveCallback );
    loadFromServer.addListener( this, &ofxRPiezosPanel::loadCallback );

    sync.setup((ofParameterGroup&)gui.getParameter(), OFXRPIEZOS_SYNC_PORT_CLIENT, serverIP, OFXRPIEZOS_SYNC_PORT_SERVER);
    
    receiver.setup( OFXRPIEZOS_DEBUG_PORT );
        
    //ofFile file("piezos.xml");
	//if ( file.exists() ) gui.loadFromFile("piezos.xml");

    // graphic setups
    datas.resize( OFXRPIEZOS_NUM_SENSORS );

    for (int i=0; i<OFXRPIEZOS_NUM_SENSORS; ++i){
        datas[i].raw = 0;
        datas[i].trigger = 0.0f;
        datas[i].triggerCounter = 0;
        datas[i].envelope = 0.0f;
        datas[i].rawPlot.resize(256);
        for( int & value : datas[i].rawPlot ) value = 0;
        datas[i].rawPlotIndex = 0;
        datas[i].envPlot.resize(256);
        for( float & value : datas[i].envPlot ) value = 0.0f;
        datas[i].envPlotIndex = 0;
    }
    
    settings.height = 160;
    settings.width = 220;
    settings.separator = 10;
    settings.envseparator = 40;
    settings.plotmax = maxplot;
    
    fbo.allocate( settings.width*4 + settings.separator*3 +2+ settings.envseparator*2 +2, settings.height*4 + settings.separator*3 + 2 );
}


void ofxRPiezosPanel::update(){
    
    sync.update();
    if(saveOnServer) saveOnServer = false;
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
        
        if ( m.getAddress() == rawAddress ){
            int channel = m.getArgAsInt32(0);
            datas[channel].raw = m.getArgAsInt32(1);
        } else if (m.getAddress() == trigAddress ){
            int channel = m.getArgAsInt32(0);
            datas[channel].trigger = m.getArgAsFloat(1);
            datas[channel].triggerCounter = 8;
        } else if (m.getAddress() == envAddress ){
            int channel = m.getArgAsInt32(0);
            datas[channel].envelope = m.getArgAsFloat(1);
        }
	}
    
    for( size_t i=0; i<datas.size(); ++i){
        datas[i].rawPlotIndex--;
        if( datas[i].rawPlotIndex<0 ) datas[i].rawPlotIndex = datas[i].rawPlot.size()-1;
        datas[i].rawPlot[datas[i].rawPlotIndex] = datas[i].raw;
       
        datas[i].envPlotIndex--;
        if( datas[i].envPlotIndex<0 ) datas[i].envPlotIndex = datas[i].envPlot.size()-1;
        datas[i].envPlot[datas[i].envPlotIndex] = datas[i].envelope;
    }

    fbo.begin();
        
        ofClear(0, 0, 0, 255);
        ofTranslate( settings.separator, 1 );
        ofNoFill();    
    
        for( size_t i=0; i<datas.size(); ++i){

            if( piezos[i].bActive ) {
                ofSetColor(255);
            }else {
                ofSetColor(120);
            }

            if( piezos[i].bTrigger && piezos[i].bActive){
                int miny =ofMap( piezos[i].gateLo, 0, settings.plotmax, settings.height, 0, true );
                int maxy =ofMap( piezos[i].gateHi, 0, settings.plotmax, settings.height, 0, true );
                        
                if( datas[i].triggerCounter > 0){
                    ofSetColor( datas[i].trigger*100 + 155 );
                    ofFill();
                    ofDrawRectangle( 0, maxy, settings.width, miny-maxy );
                    ofNoFill();
                    ofSetColor(255);
                    datas[i].triggerCounter--;
                }

                ofDrawLine( 0, miny, settings.width, miny );
                ofDrawLine( 0, maxy, settings.width, maxy );
            }

            ofDrawRectangle(0, 0, settings.width, settings.height);

            
            ofBeginShape();
            int max = datas[i].rawPlot.size();
            for( int k=0; k<max; ++k ){
                int index = datas[i].rawPlotIndex + k;
                if( index >= max ) index -= max;
                ofVertex(   ofMap( k, max, 0, 0, settings.width ), 
                            ofMap( datas[i].rawPlot[index], 0, settings.plotmax, settings.height, 0, true ) );
            }
            ofEndShape();
            
            if(piezos[i].bEnvelope ){
                float ex = settings.width + settings.envseparator;
                float ey = ofMap( piezos[i].envHi, 0, settings.plotmax, settings.height, 0, true );
                float eh = ofMap( piezos[i].envHi - piezos[i].envLo, 0, settings.plotmax, 0, settings.height, true );
                float emin = ey+eh;
                
                ofDrawLine( settings.width, ey,   ex, 0);
                ofDrawLine( settings.width, emin, ex, settings.height);
                
                ofDrawRectangle( ex, 0, settings.width, settings.height );
                ofBeginShape();
                for( int k=0; k<max; ++k ){
                    int index = datas[i].envPlotIndex + k;
                    if( index >= max ) index -= max;
                    ofVertex(   ex + ofMap( k, max, 0, 0, settings.width ), 
                                ofMap( datas[i].envPlot[index], 0, 1.0f, settings.height, 0, true ) );
                }
                ofEndShape();
            }
            

            ofDrawBitmapString( "piezo "+ofToString(i), 5, 15 );
            
            ofTranslate( 0, settings.height+settings.separator );
            if( i%3==2 ){
                ofTranslate( settings.width*2 + settings.separator*2 + settings.envseparator, -3*(settings.height+settings.separator));
            }
            
        }
    fbo.end();
}

void ofxRPiezosPanel::draw(){
    gui.draw();
    fbo.draw( gui.getPosition().x + 220, gui.getPosition().y );
}

void ofxRPiezosPanel::saveCallback( bool & value ) {
    if( value ){
        cout<< "[rPiezos] saving settings on the rPi server\n";
        bCalibrate = false;
    }
}

void ofxRPiezosPanel::loadCallback( bool & value ) {
    if( value ){
        cout<< "[rPiezos] retrieving settings from the rPi server\n";
        //ofSleepMillis(500);
    }
}
