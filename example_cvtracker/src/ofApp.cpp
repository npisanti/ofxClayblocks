#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("cvtracker receiver class");

	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);

    receiver.setup( 12345, "192.168.0.42" );

    gui.setup("gui", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( receiver.parameters );

    // changes made here will be reflected on the server device
    receiver.sendImage = 1;

}

//--------------------------------------------------------------
void ofApp::update(){
    receiver.update();
    //receiver.simulate();
}


//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground( 0 );

    receiver.draw( 0, 0 );

    gui.draw();
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