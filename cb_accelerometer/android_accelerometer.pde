// ----------- SETUP ----------------------------
String destinationIP = "192.168.1.4";
int    port          = 12345;
String id            = "phone";
float  deadzone      = 0.7;
float  boundary      = 6.0;
String savename      = "\\sdcard\\cb_accelerometer_settings.txt";
//-----------------------------------------------
// FAST TAP THREE TIMES THE SCREEN TO SET THE PARAMETERS
// optionally you can save them if you have an SD card

import ketai.sensors.*;
import ketai.ui.*;
import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;
KetaiSensor sensor;
float ax, ay, az; // accelerometer xyz
float bx, by, bz; // base xyz
boolean fax0, fay0, faz0;

boolean bSettings;
boolean bCalibrate;

void setup()
{
    try { 
        String[] lines = loadStrings( savename );
        id = lines[0];
        destinationIP = lines[1];
        port = parseInt(lines[2]);
        bx = parseFloat(lines[3]);
        by = parseFloat(lines[4]);
        bz = parseFloat(lines[5]);
        println("found settings file");
    } catch (Exception e){ 
        bx = 0.0;
        by = 0.0;
        bz = 0.0;   
    }
    
    fax0 = fay0 = faz0 = false;
    
    fullScreen();
    oscP5 = new OscP5(this, 42);
    myRemoteLocation = new NetAddress( destinationIP, port);
    sensor = new KetaiSensor(this);
    sensor.start();
    orientation( PORTRAIT );
    textAlign( CENTER, CENTER );
    textSize(displayDensity * 12);

    bSettings = false;
    bCalibrate = false;
    settingsSetup();
}

void draw()
{
    
  if(!bSettings){
      
      background( 0 );

      float x = map(ax, -boundary, boundary, 0, width );
      float y = map(ay, -boundary, boundary, 0, height );
      float side = map(az, -boundary, boundary, width*0.15, width*0.4 );
      
      noFill();
      stroke(255);
      rect(0, 0, width-1, height-1);
      rect((width-side)*0.5, (height-side)*0.5, side, side);
      
      fill(255);

      line( x, 0, x, height );
      line( 0, y, width, y );
  
      textAlign( LEFT, TOP );
      text( "/"+ id + "/ax : " + nfp(ax, 1, 3) + "\n" +
            "/"+ id + "/ay : " + nfp(ay, 1, 3) + "\n" +
            "/"+ id + "/az : " + nfp(az, 1, 3),
            10, 10 );
        
    } else {
        settingsDraw();
    }
}

void onAccelerometerEvent(float x, float y, float z)
{
    if(bCalibrate){ bx = x; by = y;  bz = z; }
    
    ax = x - bx;
    ay = y - by;
    az = z - bz;
    if(ax < -deadzone){ ax+=deadzone; }else if(ax > deadzone){ ax-=deadzone; }else { ax=0.0; }
    if(ay < -deadzone){ ay+=deadzone; }else if(ay > deadzone){ ay-=deadzone; }else { ay=0.0; }
    if(az < -deadzone){ az+=deadzone; }else if(az > deadzone){ az-=deadzone; }else { az=0.0; }
    
    if(ax!=0.0 ){ 
        ax = -ax; // positive to the right
        OscMessage myMessage = new OscMessage("/"+ id + "/ax");
        myMessage.add( ax );
        oscP5.send(myMessage, myRemoteLocation);
        fax0 = true;
    }else if( fax0 ){ 
        OscMessage myMessage = new OscMessage("/"+ id + "/ax");
        myMessage.add( 0.0 );
        oscP5.send(myMessage, myRemoteLocation);
        fax0 = false;
    }
    if(ay!=0.0 ){ 
        OscMessage myMessage = new OscMessage("/"+ id + "/ay");
        myMessage.add( ay );
        oscP5.send(myMessage, myRemoteLocation);
        fay0 = true;
    }else if( fay0 ){ 
        OscMessage myMessage = new OscMessage("/"+ id + "/ay");
        myMessage.add( 0.0 );
        oscP5.send(myMessage, myRemoteLocation);
        fay0 = false;
    }
    if(az!=0.0 ){ 
        OscMessage myMessage = new OscMessage("/"+ id + "/az");
        myMessage.add( az );
        oscP5.send(myMessage, myRemoteLocation);
        faz0 = true;
    }else if( faz0 ){ 
        OscMessage myMessage = new OscMessage("/"+ id + "/az");
        myMessage.add( 0.0 );
        oscP5.send(myMessage, myRemoteLocation);
        faz0 = false;
    }
}