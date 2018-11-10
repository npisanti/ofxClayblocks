#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("rpiezos receiver class");
    
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);

    piezos.start( 12345 );

}

//--------------------------------------------------------------
void ofApp::update(){
    // this will also clear the triggers
    piezos.logTriggers();
    
}


//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground( 0 );

    piezos.drawEnvelopes( 0, 0, ofGetWidth() );

}


//--------------------------------------------------------------
void ofApp::exit(){
    
    piezos.stop();
    
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
