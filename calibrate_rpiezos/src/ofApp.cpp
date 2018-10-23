#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //panel.setup( "169.254.0.2" ); // set the raspberry pi IP here
    panel.setupFromFile( "ip.xml" ); // setup from file ( set IP in bin/data/ip.xml )
    
    ofBackground(0);
    ofDisableAntiAliasing();
    ofSetWindowTitle( "rPiezos calibration utility" );
}

//--------------------------------------------------------------
void ofApp::update(){
    panel.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor( 255, 85, 85 );
    panel.draw();
    ofDrawBitmapString("this is a program to setup your piezos, first load the settings from the rPi server clicking on ''load server settings''\nthe ''calibrate piezos'' box enable the calibration mode (OSC is sent to this app instead of the client port)\nwhen you're done save your settings by clicking on ''save server settings''\nthe rPi server automatically load the lastly stored settings, but always disactivate the calibration mode when it boots\nports and IPs are setted by manually changing configuration files or by hardcoding them changing the sources", 250, ofGetHeight()-65 );
}

//--------------------------------------------------------------
void ofApp::exit(){
    if( panel.bCalibrate ){
        ofLogNotice()<<"calibration utility: deactivating calibration mode before quitting";
        panel.bCalibrate = false;
        panel.update();
        ofSleepMillis(50);
    }
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
