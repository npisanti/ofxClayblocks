
#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxRPiezoParams.h"
#include "ofxRPiezosConstants.h"
#include "ofxOscParameterSync.h"

#include "ofxXmlSettings.h"
#include "ofxGui.h"


struct PiezoGraphicsData {

    int raw;
    float trigger;
    int triggerCounter;
    float envelope;

    vector<int> rawPlot;
    int rawPlotIndex;
    vector<float> envPlot;
    int envPlotIndex;

};


class ofxRPiezosPanel {

public:

    struct GraphicsSettings {
        int width;
        int height;
        int separator;
        int envseparator;
        int plotmax;
    };

    void setupFromFile( string xmlSettingsPath );
    void setup( string serverIP, string name="piezos", int maxplot=127 );
    void update( );
    void draw();

    ofxPanel gui;

    vector<ofxRPiezoParams> piezos;

    ofParameter<bool> bCalibrate;
    ofParameter<bool> saveOnServer;
    ofParameter<bool> loadFromServer;

    GraphicsSettings settings;

    ofFbo fbo;

private:
    void saveCallback( bool & value );
    void loadCallback( bool & value );

    ofxOscParameterSync sync;

    ofxOscReceiver receiver;

    // graphic variables:
    vector<PiezoGraphicsData> datas;

    string rawAddress;
    string envAddress;
    string trigAddress;
};
