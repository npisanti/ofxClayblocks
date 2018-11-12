#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("CV Trackers");

	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);
    
    tracker.setup( "localhost" );
    //tracker.setup( "192.168.0.42" ); // set server address

    gui.setup("gui", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( tracker.parameters );
    
    // this will update settings on the server
    gui.loadFromFile("settings.xml"); 
    
    // changes made here will be reflected on the server device
    //tracker.sendImage = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
    tracker.update();
}


//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground( 0 );

    tracker.draw( 20, 20 );

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
