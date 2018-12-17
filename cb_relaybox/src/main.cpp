#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxGPIO.h"
#include "ofxOsc.h"

class noWin : public ofBaseApp{
public:

    struct Command {
        ofParameterGroup parameters;
            ofParameter<string> address;
            ofParameter<int> num;
        GPIO gpio;
    };

    std::vector<ofxOscReceiver>  receivers;
    
    ofParameterGroup parameters;
    vector<Command> commands;
    

    //-------------------------------------------------------------------------
    void setup(){
            
        ofSetVerticalSync(false);
                
        // -------------- LOAD GPIO SETTINGS -----------------------------------
        parameters.setName( "relaybox" );
        commands.resize( 8 );
        for(size_t i = 0; i < commands.size(); i++){
            commands[i].parameters.setName( "command " + ofToString(i) );
            commands[i].parameters.add( commands[i].address.set("address", "/relay/k" + ofToString(i+1)));
            commands[i].parameters.add( commands[i].num.set("gpio", 17+i, 0, 40) );
            parameters.add( commands[i].parameters );
        }
        
        ofJson json = ofLoadJson("settings.json");
        ofDeserialize( json, parameters );

        for(size_t i = 0; i < commands.size(); i++){
            commands[i].gpio.setup( commands[i].num );
            commands[i].gpio.export_gpio();
            commands[i].gpio.setdir_gpio("out");
            cout<< "[cb_relaybox] osc="<<commands[i].address<< " | gpio=" << commands[i].num << "\n";
        }

        //ofJson json;
        //ofSerialize( json, parameters );
        //ofSavePrettyJson( "settings.json", json );
        
        // ------------- SET UP RECEIVERS --------------------------------------
        // fix just for this: use multiple ofxReceivers with different ports
        const auto & myIPs = ofSplitString( ofSystem( "hostname -I" ), " " );
       
        receivers.resize( myIPs.size()-1 ); // command returns empty last pos
        
        for( size_t i=0; i<myIPs.size()-1; ++i ){
            auto & ip = myIPs[i];
            std::cout<<"[cb_relaybox] IP address: "<<ip << "\n";
            
            const auto & addressNumbers = ofSplitString( ip, "." );
            
            int port = 2000 + ofToInt(addressNumbers[3]);
            receivers[i].setup( port );
            
            std::cout<<"[cb_relaybox] listening on port "<<port<< "\n";
        }

    }

    //-------------------------------------------------------------------------
    void update() {
        for( auto & receiver : receivers ){
            while(receiver.hasWaitingMessages()){
                // get the next message
                ofxOscMessage m;
                receiver.getNextMessage(m);
                for(size_t i=0; i<commands.size(); ++i){
                    if ( m.getAddress() == commands[i].address.get() ){        
                        int out = m.getArgAsInt32(0);
                        if(out==0){
                            commands[i].gpio.setval_gpio("0");
                        }else{
                            commands[i].gpio.setval_gpio("1");
                        }
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
    ofSetupOpenGL(&window, 0,0, OF_WINDOW);
    ofRunApp( app );
}
