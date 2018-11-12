#include "ofApp.h"

#define BUFFERW 160
#define BUFFERH 120 

// ------------------------------------------------------------------
void ofApp::setup() {
	
    ofSetVerticalSync(false);
    ofSetBackgroundAuto( false );
    
    width = 640; 
    height = 480;

    
    // loading settings 
    settings.setName( "settings");
        settings.add( drawToScreen.set("draw to screen", false) );
        
        camera.setName( "kinect");
            camera.add( tilt.set("tilt", 0, -30, 30) );
                tilt.addListener( this, &ofApp::onTilt );
            camera.add( useDepth.set("use depth", true) );
        settings.add( camera );
    
        settings.add( tracking.tracker );
        
        settings.add( tracking.network );        
        
    ofJson json = ofLoadJson("settings.json");
    ofDeserialize( json, settings );

    //ofJson json;
    //ofSerialize( json, settings );
    //ofSavePrettyJson( "settings.json", json );

    if( drawToScreen ){
        ofSetWindowShape( width*2, height );
    }

    if( useDepth ){
        kinect.init(false, false); // disable video image (faster fps)
    }else{
        kinect.init( true ); // setup kinect with infrared       
    }
    
    kinect.open();	
    kinect.setCameraTiltAngle( tilt );

    gray.allocate(kinect.width, kinect.height);
    ofxCv::imitate( frame, ofxCv::toCv(gray) );

    // add parameters for sync
    tracking.sync.add( tilt );
    tracking.sync.add( useDepth );
    
    // now setup the tracking 
    tracking.setup( width, height, frame );
    
}
    
void ofApp::onTilt( int & value ){
    kinect.setCameraTiltAngle( tilt );
}

// ------------------------------------------------------------------
void ofApp::update() {
    
    tracking.updateSync();

    kinect.update();
    
    if( kinect.isFrameNew() ){
        
        if( useDepth ){
            gray.setFromPixels( kinect.getDepthPixels() ); // use depth
        }else{
            gray.setFromPixels( kinect.getPixels() ); // use IR
        }
        
        frame = ofxCv::toCv( gray );
        
        tracking.update( frame );
	}
    
}

// ------------------------------------------------------------------
void ofApp::draw() {
    if( drawToScreen ){
        ofClear( 0, 0, 0, 255 );
        ofSetColor(255);
        if(!frame.empty()){
            ofxCv::drawMat( frame, 0, 0, width, height);
            ofxCv::drawMat( tracking.thresh, width, 0, width, height );
        } 
        ofTranslate( width, 0 );
        ofSetColor( 255, 0, 0 );
        tracking.finder.draw();        
    }
}

// ------------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0);
    kinect.close();
}
