#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
#ifndef GRAPHIC_DEBUG
    ofSetVerticalSync(false);
#endif
    // setup from settings file, easier to change without recompiling
    server.setup("ip.xml"); 
    
    // setup from values
    //server.setup("169.254.0.1", 4444);
}

//--------------------------------------------------------------
void ofApp::update(){
    server.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef GRAPHIC_DEBUG
    server.gui.draw();
#endif
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
