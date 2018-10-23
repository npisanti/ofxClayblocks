#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxGPIO.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

class noWin : public ofBaseApp{
public:

    struct Command {
        string osc;
        GPIO gpio;
    };

    ofxOscReceiver  receiver;
    vector<Command> commands;
    std::string path;

    //-------------------------------------------------------------------------
    void setup(){
            
        ofSetVerticalSync(false);
        
        // LOADING XML SETTINGS
        ofxXmlSettings settings;
        cout <<  "[xml settings] loading "<<path<< endl;
        if( settings.loadFile( path ) ){
            cout << "[xml settings] "<<path<<" loaded!" << endl;
        }else{
            cout << "[xml settings] error! unable to load "<<path<< endl;
            exit();
        }
        
        int port = settings.getValue("settings:port", 1234);
        receiver.setup( port );

        int numCommands = settings.getNumTags("command");
        commands.resize(numCommands);

        for(int i = 0; i < numCommands; i++){
            commands[i].osc = settings.getValue("command:osc", "/pinout", i);
            string gpioNum = settings.getValue("command:gpio", "0", i);
            
            commands[i].gpio.setup( gpioNum );
            commands[i].gpio.export_gpio();
            commands[i].gpio.setdir_gpio("out");
            
            cout<< "osc="<<commands[i].osc<< " | gpio=" << gpioNum << "\n";
        }
    }

    //-------------------------------------------------------------------------
    void update() {
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(m);
            for(size_t i=0; i<commands.size(); ++i){
                if ( m.getAddress() == commands[i].osc ){        
                    int out = m.getArgAsInt32(0);
                    if(out==0){
                        commands[i].gpio.setval_gpio("0");
                    }else{
                        commands[i].gpio.setval_gpio("1");
                    }
                }                   
            }
        }
        ofSleepMillis(1);
    }

    //-------------------------------------------------------------------------
    void exit(){
        for(size_t i=0; i<commands.size(); ++i){
            commands[i].gpio.unexport_gpio();
        }
    }

};

//-----------------------------------------------------------------------------
int main(int argc, char *argv[]){
    ofAppNoWindow window;
    noWin * app = new noWin();
    
    app->path = std::string("settings.xml");
    if(argc>1){		
		for(int i=1; i<argc; i+=2){
			if( ! strcmp( argv[i], "-c" ) ){
                app->path = std::string( argv[i+1] );	
			}
		}
	}

    ofSetupOpenGL(&window, 0,0, OF_WINDOW);
    ofRunApp( app );
}
