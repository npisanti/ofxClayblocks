#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("piezos client");
    
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);
    ofDisableAntiAliasing();

    piezos.setup( "169.254.0.2" );
    gui.setup("panel", "settings.xml", 20, 20 );
    
    // adds piezo sensors parameter groups
    for( auto & piezo : piezos.piezos ){
        gui.add( piezo.parameters );
    }
    
    // piezos.parameters is all the groups already grouped together
    // you can do this instead of adding it for each sensor
    //gui.add( piezos.parameters ); 
    
    // loading the settings from file will update all the values on the server
    gui.loadFromFile( "settings.xml" );
    gui.minimizeAll();
}

//--------------------------------------------------------------
void ofApp::update(){
    piezos.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground( 0 );
    
    gui.draw();
    
    piezos.draw( 240, 20, ofGetWidth()-260, ofGetHeight()-40, ofColor( 255, 80, 80) );
    //piezos.drawEnvelopes( 0, 0, ofGetWidth() );
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
