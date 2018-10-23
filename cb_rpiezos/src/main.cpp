#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"  

//========================================================================
int main( ){

#ifdef GRAPHIC_DEBUG
	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context
#else
    ofAppNoWindow window;  
    ofSetupOpenGL(&window, 256,256,OF_WINDOW);  
#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
