#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"  

//========================================================================
int main( int argc, char *argv[] ){

    ofAppNoWindow window;  
    
    ofApp *app = new ofApp();

    app->cliInput = false;

    if(argc>3){		
        for( int i=1; i<argc; ++i ){
            switch( i ){
                case 1: app->rpiIP = std::string( argv[i] ); break;
                case 2: app->clientIP = std::string( argv[i] ); break;
                case 3: app->devname = std::string( argv[i] ); break;
                default : break;
            }
        }   
        app->cliInput = true;
    }

    ofSetupOpenGL(&window, 256,256,OF_WINDOW);  
	ofRunApp( app );

}
