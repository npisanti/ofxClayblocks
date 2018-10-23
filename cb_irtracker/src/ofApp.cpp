
#include "ofApp.h"

    /*
        auto white balance modes:
            0] = "off";
            1] = "auto";
            2] = "sunlight";
            3] = "cloudy";
            4] = "shade";
            5] = "tungsten";
            6] = "fluorescent";
            7] = "incandescent";
            8] = "flash";
            9] = "horizon";
            10] = "max"; 
    
        saturation, sharpness and contrast goes to -100 to 100
        brightness should go from 0 to 100

        exposure modes:
            0] = "off";
            1] = "auto";
            2] = "night";
            3] = "night preview";
            4] = "backlight";
            5] = "spotlight";
            6] = "sports";
            7] = "snow";
            8] = "beach";
            9] = "very long";
            10] = "fixed fps";
            11] = "antishake";
            12] = "fireworks";
            13] = "max";
    */


#define CVTRACK_RED 2

// ------------------------------------------------------------------
void ofApp::setup() {
	
    ofSetVerticalSync(false);
    ofSetBackgroundAuto( false );
    
    // loading settings 
    settings.setName( "settings");
        settings.add( drawToScreen.set("draw to screen", false) );
    
        camera.setName( "camera");
            camera.add( width.set("width", 640, 640, 1920) );
            camera.add( height.set("height", 480, 480, 1080) );
            camera.add( file.set("calibration file", "pimoroni_fisheye_640x480.yml") );
            camera.add( useRedChannel.set("use red channel", false) );
            camera.add( saturation.set("saturation",0,-100,100) );
            camera.add( sharpness.set("sharpness",0,-100,100) );
            camera.add( contrast.set("contrast",0,-100,100) );
            camera.add( brightness.set("brightness",50,0,100) );
            camera.add( awbMode.set("auto white balance mode", 1,0,10) );
            camera.add( exposureMode.set("exposure mode",0,0,13) );
        settings.add( camera );

        settings.add( tracking.parameters );
        
        settings.add( tracking.network );
        
    ofJson json = ofLoadJson("settings.json");
    ofDeserialize( json, settings );

    //ofJson json;
    //ofSerialize( json, settings );
    //ofSavePrettyJson( "settings.json", json );

    // cam setup
    if( useRedChannel ){         
        cam.setup( width, height, true ); // use color, get red channel to use included blue gels   
    }else{
        cam.setup( width, height, false ); 
    }

    int mode = awbMode;
    cam.setAWBMode( (MMAL_PARAM_AWBMODE_T) mode ); // auto white balance off
    mode = exposureMode;
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T) mode );
    cam.setBrightness( brightness );
    cam.setSharpness( sharpness );
    cam.setContrast( contrast );
    cam.setSaturation( saturation );

	calibration.setFillFrame( true ); // true by default
    calibration.load( file );
    

    bool hasFrame = false;
    
    while(!hasFrame){
        frame = cam.grab();
        
        if( !frame.empty() ){
            
            if(useRedChannel){
                cv::split ( frame, channels );
                red = channels[CVTRACK_RED];
                ofxCv::imitate( undistorted, red);
                // sets ups tracking with ports loaded from settings
                tracking.setup( width, height, red );                
            }else{
                ofxCv::imitate( undistorted, frame);
                // sets ups tracking with ports loaded from settings
                tracking.setup( width, height, frame );
            }
      
            hasFrame = true;
        }
    }

}


// ------------------------------------------------------------------
void ofApp::update() {
    
    tracking.sync();
    
    frame = cam.grab();
    
    if(!frame.empty()){
        
        if(useRedChannel ){
            cv::split ( frame, channels );
            red = channels[CVTRACK_RED];
            calibration.undistort( red, undistorted );
        }else{
            calibration.undistort( frame, undistorted );
        }

        tracking.update( undistorted );
	}
    
}



// ------------------------------------------------------------------
void ofApp::draw() {
    if( drawToScreen ){
        ofClear( 0, 0, 0, 255 );
        ofSetColor(255);
        if(!frame.empty()){
            ofxCv::drawMat( undistorted, 0, 0, width/2, height/2);
            ofxCv::drawMat( tracking.thresh, width/2, 0, width/2, height/2);
        } 
        ofTranslate( width/2, 0 );
        ofScale( 0.5f );
        ofSetColor( 255, 0, 0 );
        tracking.finder.draw();        
    }
}
