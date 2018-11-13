
int keyMode;
int uiH;
int timer;
int tapCount;
String typing;

void settingsSetup() {
    keyMode = -1;
    uiH = height / 9;
    timer = 0;
    typing = "";
}

void settingsDraw() {
    background(0);
    fill(0);
    stroke(255);
    rect(0, 0, width-1, uiH);
    rect(0, uiH, width-1, uiH);
    rect(0, uiH*2, width-1, uiH);
    rect(0, uiH*3, width-1, uiH);
    rect(0, uiH*4, width/2, uiH);
    rect(width/2, uiH*4, width/2 -2, uiH);

    textAlign( CENTER, CENTER );
    fill(255);
    text("set id\nid : " + id, 0, 0, width, uiH);
    text("set destination IP\nIP : " + destinationIP, 0, uiH, width, uiH);
    text("set port\nport : " + port, 0, uiH*2, width, uiH);
    text("calibrate: \n" +
        "bx : " + nfp(bx, 1, 3) + "\n" +
        "by : " + nfp(by, 1, 3) + "\n" +
        "bz : " + nfp(bz, 1, 3), 0, uiH*3, width, uiH);
    text("back", 0, uiH*4, width/2, uiH);
    text("save settings\n(sd card needed)", width/2, uiH*4, width/2 -2, uiH);
    
    if (keyMode!=-1) {
        textAlign( LEFT, TOP );
        if ( millis()%800>400 ) {
            text( typing, 0, 12 + uiH*5, width, uiH);
        } else {
            text( typing + "_", 0, 12 + uiH*5, width, uiH);
        }
    }
}

void mousePressed() {
    if (!bSettings) {
        int now = millis();
        int delta = now - timer;
        timer = now;
        if (delta < 600 ) {
            tapCount++;
            if (tapCount == 3) {
                bSettings = true;
            }
        } else {
            tapCount = 0;
        }
    } else {
        int option = mouseY / uiH;

        switch(option) {
            case 0: case 1: case 2:
                if(keyMode == -1){
                    keyMode = option;
                }else{
                    keyMode = -1;
                }
                KetaiKeyboard.toggle(this);
            break;
    
            case 3:
                bCalibrate = true;
            break;
    
            case 4:
                if(mouseX > width/2){
                    println("saving to file");
                    String[] settings = new String [6];
                    settings[0] = id;
                    settings[1] = destinationIP;
                    settings[2] = str( port );
                    settings[3] = str( bx );
                    settings[4] = str( by );
                    settings[5] = str( bz );
                    saveStrings( savename, settings );  
                }
                bSettings = false;
            break;
        }
    }
}
void mouseReleased() {
    if (bSettings && bCalibrate) {
        bCalibrate = false;
    }
}


public void keyPressed() {

    if ( bSettings && keyMode!=-1) {
        
        if(key==10) {
            switch(keyMode) {
                case 0:
                    id = typing;
                break;
                case 1:
                    destinationIP = typing;
                    myRemoteLocation = new NetAddress( destinationIP, port);
                break;
                case 2:
                    port = parseInt(typing);
                    myRemoteLocation = new NetAddress( destinationIP, port);
                break;        
                }    
            typing = "";
            keyMode = -1;
            KetaiKeyboard.toggle(this);
        
        }else if( key==65535 && typing.length()>0 ){
            typing = typing.substring(0, typing.length() - 1);
        }else{
            typing += (char) key;
        }
    }   
}
