
// MPR121 to OSC messages
// the OSC messages have to parameters
// the first is an int for the channel
// the second is 1 for touched and 0 for released

#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxGPIO.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"


// The default I2C address
#define MPR121_I2CADDR_DEFAULT 0x5A

#define MPR121_TOUCHSTATUS_L 0x00
#define MPR121_TOUCHSTATUS_H 0x01
#define MPR121_FILTDATA_0L  0x04
#define MPR121_FILTDATA_0H  0x05
#define MPR121_BASELINE_0   0x1E
#define MPR121_MHDR         0x2B
#define MPR121_NHDR         0x2C
#define MPR121_NCLR         0x2D
#define MPR121_FDLR         0x2E
#define MPR121_MHDF         0x2F
#define MPR121_NHDF         0x30
#define MPR121_NCLF         0x31
#define MPR121_FDLF         0x32
#define MPR121_NHDT         0x33
#define MPR121_NCLT         0x34
#define MPR121_FDLT         0x35

#define MPR121_TOUCHTH_0    0x41
#define MPR121_RELEASETH_0    0x42
#define MPR121_DEBOUNCE 0x5B
#define MPR121_CONFIG1 0x5C
#define MPR121_CONFIG2 0x5D
#define MPR121_CHARGECURR_0 0x5F
#define MPR121_CHARGETIME_1 0x6C
#define MPR121_ECR 0x5E
#define MPR121_AUTOCONFIG0 0x7B
#define MPR121_AUTOCONFIG1 0x7C
#define MPR121_UPLIMIT   0x7D
#define MPR121_LOWLIMIT  0x7E
#define MPR121_TARGETLIMIT  0x7F

#define MPR121_GPIODIR  0x76
#define MPR121_GPIOEN  0x77
#define MPR121_GPIOSET  0x78
#define MPR121_GPIOCLR  0x79
#define MPR121_GPIOTOGGLE  0x7A



#define MPR121_SOFTRESET 0x80



class noWin : public ofBaseApp{
public:
    
    I2c * bus;
    ofxOscSender sender;
    string id;
    vector<int> touches;
    vector<int> touches_m1;
    bool verbose;
    string path;
    
    void setThresholds( uint8_t touch, uint8_t release ){
        for (uint8_t i=0; i<12; i++) {
            bus->writeByte(MPR121_TOUCHTH_0 + 2*i, touch);
            bus->writeByte(MPR121_RELEASETH_0 + 2*i, release);
        }
    }
    
    
    uint16_t touched() {
        uint8_t data [2];
        bus->readBlock(MPR121_TOUCHSTATUS_L, 2, data);
        uint16_t t = data[0] + data[1]*256;
        return t & 0x0FFF;
    }


    void setup(){
        ofSetVerticalSync(false);
        
        // set up variables
        ofxXmlSettings settings;
        cout <<  "[xml settings] loading "<<path<< endl;
        if( settings.loadFile( path ) ){
            cout << "[xml settings] "<<path<<" loaded!" << endl;
        }else{
            cout << "[xml settings] error! unable to load "<<path<< endl;
            exit();
        }
        
        string ip = settings.getValue("settings:client_ip", "localhost");
        int port = settings.getValue("settings:port", 4444);   
        int threshold = settings.getValue("settings:threshold", 12);   
        int release = settings.getValue("settings:release", 6);   
        id = settings.getValue("settings:id", "cap");
        id = "/" + id;
        verbose = settings.getValue("settings:verbose", 0);        
        
        
        // SET UP OSC
        sender.setup( ip, port );

        // SET UP i2c
        bus = new I2c("/dev/i2c-1");
        bus->addressSet(MPR121_I2CADDR_DEFAULT);
        
        // soft reset
        bus->writeByte(MPR121_SOFTRESET, 0x63);
        usleep(1000); // 1ms sleep

        bus->writeByte(MPR121_ECR, 0x0);

        uint8_t c =  bus->readByte(MPR121_CONFIG2);
        if (c != 0x24) {
            ofLogError()<<"failed to initialize";
        } else {

            setThresholds(threshold, release);

            bus->writeByte(MPR121_MHDR, 0x01);
            bus->writeByte(MPR121_NHDR, 0x01);
            bus->writeByte(MPR121_NCLR, 0x0E);
            bus->writeByte(MPR121_FDLR, 0x00);

            bus->writeByte(MPR121_MHDF, 0x01);
            bus->writeByte(MPR121_NHDF, 0x05);
            bus->writeByte(MPR121_NCLF, 0x01);
            bus->writeByte(MPR121_FDLF, 0x00);

            bus->writeByte(MPR121_NHDT, 0x00);
            bus->writeByte(MPR121_NCLT, 0x00);
            bus->writeByte(MPR121_FDLT, 0x00);

            bus->writeByte(MPR121_DEBOUNCE, 0);
            bus->writeByte(MPR121_CONFIG1, 0x10); // default, 16uA charge current
            bus->writeByte(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period

            //  bus->writeByte(MPR121_AUTOCONFIG0, 0x8F);

            //  bus->writeByte(MPR121_UPLIMIT, 150);
            //  bus->writeByte(MPR121_TARGETLIMIT, 100); // should be ~400 (100 shifted)
            //  bus->writeByte(MPR121_LOWLIMIT, 50);
            
            // enable all electrodes
            bus->writeByte(MPR121_ECR, 0x8F);  // start with first 5 bits of baseline tracking
        }
        
        touches.resize(12);
        touches_m1.resize(12);
        for(size_t i=0; i<touches.size(); ++i){
            touches[i] = touches_m1[i] = 0;
        }
    }
    
    void update(){
        
        uint16_t touchbytes = touched();

        for (int i=0; i<12; i++) {

            uint16_t i_touched = (touchbytes >> i) & 0x0001;

            touches[i] = (int) i_touched;
            
            if(touches[i] != touches_m1[i] ){
                //ofLog()<< "cap sensor " << i << " value= "<<touches[i];
                ofxOscMessage m;
                m.setAddress( id );
                m.addIntArg( i );
                m.addIntArg( touches[i] );
                sender.sendMessage(m, false);
            }
            
            touches_m1[i] = touches[i];
        }
        
        ofSleepMillis(1);
    }

};

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
