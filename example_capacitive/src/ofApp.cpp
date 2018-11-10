#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("capacitive");
    
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);

    capacitive.setup( "192.168.0.22" );

    gui.setup("gui", "settings.xml", 20, 20 );
    gui.add( capacitive.parameters );
}

//--------------------------------------------------------------
void ofApp::update(){
    capacitive.update();
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( 0 );
    gui.draw();

    float h = 100;    
    float y = ofGetHeight() - h;
    float space = ofGetWidth() / 12.0f;
    
    ofFill();
    ofSetColor(255);
    for( int i=0; i<12; ++i){
        if( capacitive.touched(i) ){
            ofDrawRectangle( i*space, y, space, h );
        }
    }
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
