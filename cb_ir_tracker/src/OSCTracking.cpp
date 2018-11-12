
#include "OSCTracking.h"

#define BUFFERW 160
#define BUFFERH 120

np::helper::OSCTracking::OSCTracking(){
    blobs.reserve(128); // more than enough
    blobs.clear();

    finder.setThreshold(127);

    buffersize = BUFFERW*BUFFERH;
    buffer = new char[buffersize];

    sync.setName( "sync");
        sync.add( doBackgroundSubtraction.set("background subtraction", false) );
        sync.add( takeBackground.set("take background", true) );
        sync.add( denoise.set("denoise", false) );
        sync.add( thresholdLow.set("threshold low", 0, 0, 255) );
        sync.add( thresholdHigh.set("threshold high", 255, 0, 255) );
        sync.add( minArea.set("area min", 20, 1, 5000) );
        sync.add( maxArea.set("area max", 15000, 1, 50000) );;
        sync.add( persistence.set("persistence", 15, 1, 100) );
        sync.add( maxDistance.set("max distance", 32, 1, 100) );
        sync.add( filterDeltaDistance.set("filter delta distance", 5, 0, 80) );
        sync.add( filterDeltaVelocity.set("filter delta velocity", 0.01f, 0.00001f, 1.0f) );
        sync.add( filterRatio.set("filter ratio", 10.0f, 1.0f, 25.0f) );
        sync.add( filterMinX.set("filter min X", 0.0f, 0.0f, 1.0f) );
        sync.add( filterMaxX.set("filter max X", 1.0f, 0.0f, 1.0f) );
        sync.add( filterMinY.set("filter min Y", 0.0f, 0.0f, 1.0f) );
        sync.add( filterMaxY.set("filter max Y", 1.0f, 0.0f, 1.0f) );
        sync.add( sendContours.set( "send contours", false ) );
        sync.add( simplifyContours.set( "simplify contours", 0.6f, 0.0f, 2.0f ) );
        sync.add( sendImage.set("send image", 0, 0, 2) );
    
    tracker.setName("tracker" );
        tracker.add( doBackgroundSubtraction );
        tracker.add( takeBackground );
        tracker.add( denoise );
        tracker.add( thresholdLow );
        tracker.add( thresholdHigh );
        tracker.add( minArea );
        tracker.add( maxArea );
        tracker.add( persistence );
        tracker.add( maxDistance);
        tracker.add( filterDeltaDistance );
        tracker.add( filterDeltaVelocity );
        tracker.add( filterRatio );
        tracker.add( filterMinX );
        tracker.add( filterMaxX );
        tracker.add( filterMinY );
        tracker.add( filterMaxY );
        tracker.add( sendContours );
        tracker.add( simplifyContours );
        tracker.add( sendImage );    
        
    network.setName("network");
        network.add( clientIP.set("client ip", "localhost") );
        
    // setup group for OSC parameter sync 

    //sync.add( doBackgroundSubtraction );
    //sync.add( sendImage );
}

np::helper::OSCTracking::~OSCTracking(){
    delete [] buffer;
}

void np::helper::OSCTracking::setup( int width, int height, const cv::Mat & toImitate ){
    this->width = width;
    this->height = height;

    // --------------- generating ports ------------------------------------ 
    int port = 12345;
    
    const auto & myIPs = ofSplitString( ofSystem( "hostname -I" ), " " );

    // check for similarity to the clientIP
    int chosen = 0;
    int greatestSim = 0;
    for( size_t i=0; i<myIPs.size(); ++i ){
        int min = (clientIP.get().size() < myIPs[i].size()) ? clientIP.get().size() : myIPs[i].size();
        int similarity = 0;
        for(int k=1; k<min; ++k){
            if( clientIP.get().substr(0, k) == myIPs[i].substr(0, k) ){
                similarity++;
            }
        }
        if( similarity > greatestSim ){
            greatestSim = similarity;
            chosen = i;
        }
    }
        // in case of localhost, the first will be chosen 
    std::string portGenerator = myIPs[chosen]; 
    
    const auto & addressNumbers = ofSplitString( portGenerator, "." );
    port = 2000 + ofToInt(addressNumbers[3]);
    int serverSyncSend = 3000 + ofToInt(addressNumbers[3]);
    int serverSyncReceive = 4000 + ofToInt(addressNumbers[3]);
    
    // ------------------ OSC setup --------------------------------------------
        
    sender.setup( clientIP, port );
    std::cout<<"[cb_kinectracker] sending OSC to "<<clientIP<<" on port "<<port<<"\n"; 
       
    synchronizer.setup( sync, serverSyncReceive, clientIP, serverSyncSend );
    std::cout<<"[cb_kinectracker] syncing parameters on port "<<serverSyncSend<<" (send) and port "<<serverSyncReceive<<" (receive)\n";

    divideW = 1.0f / float(width);
    divideH = 1.0f / float(height);

    ofxCv::imitate( tLow, toImitate );
    ofxCv::imitate( tHigh, toImitate );
    ofxCv::imitate( thresh, toImitate );
    ofxCv::imitate( background, toImitate );
    
    takeBackground = true;

}

void np::helper::OSCTracking::updateSync(){
    synchronizer.update();
}

void np::helper::OSCTracking::update( cv::Mat & frame ){

    if( takeBackground ){
        background = frame;
        takeBackground = false;
    }

    if( doBackgroundSubtraction ){
        cv::Mat subtracted;
        cv::subtract( frame, background, subtracted );
        frame = subtracted;
    }

    if(denoise){
        cv::medianBlur( frame, frame, 3 );
    }

    cv::threshold( frame, tLow,   thresholdLow, 255,  0 );
    cv::threshold( frame, tHigh,  thresholdHigh, 255, 1 );
    cv::bitwise_and(tLow, tHigh, thresh );

    // set contour tracker parameters
    finder.setMinArea(minArea);
    finder.setMaxArea(maxArea);
    finder.setFindHoles( false );

    finder.getTracker().setPersistence(persistence);
    finder.getTracker().setMaximumDistance(maxDistance);


    switch( sendImage ){
        case 1:
        {
            for( int x=0; x<BUFFERW; ++x ){
                for (int y=0; y<BUFFERH; ++y){
                    int mx = ofMap( x, 0, BUFFERW, 0, width, true );
                    int my = ofMap( y, 0, BUFFERH, 0, height, true );
                    buffer[x+y*BUFFERW] = frame.at<char>( my, mx );
                }
            }

            ofBuffer ofbuf;
            ofbuf.clear();
            ofbuf.append(buffer, buffersize);

            ofxOscMessage m;
            m.setAddress( "/cvtracker/image" );
            m.addBlobArg(ofbuf);
            sender.sendMessage(m, false);
        }
        break;

        case 2:
        {
            for( int x=0; x<BUFFERW; ++x ){
                for (int y=0; y<BUFFERH; ++y){
                    int mx = ofMap( x, 0, BUFFERW, 0, width, true );
                    int my = ofMap( y, 0, BUFFERH, 0, height, true );
                    buffer[x+y*BUFFERW] = thresh.at<char>( my, mx );
                }
            }

            ofBuffer ofbuf;
            ofbuf.clear();
            ofbuf.append(buffer, buffersize);

            ofxOscMessage m;
            m.setAddress( "/cvtracker/image" );
            m.addBlobArg(ofbuf);
            sender.sendMessage(m, false);
        }
        break;

        default: break;
    }

    // determine found contours
    finder.findContours( thresh );
    doBlobs();
}


void np::helper::OSCTracking::doBlobs(){

    float x0 = filterMinX*width;
    float x1 = filterMaxX*width;
    float y0 = filterMinY*height;
    float y1 = filterMaxY*height;

    for ( auto & blob : blobs ){ blob.found = false; }


    for( size_t i=0; i<finder.size(); ++i ){
        
        float rw = finder.getBoundingRect(i).width;
        float rh = finder.getBoundingRect(i).height;
        
        if( finder.getCenter(i).x > x0 && finder.getCenter(i).x < x1 && finder.getCenter(i).y > y0 && finder.getCenter(i).y < y1 && ( ( rw / rh )<filterRatio ) && ( ( rh / rw )<filterRatio ) ){

            bool insert = true;

            for( auto & blob : blobs ){

                if( finder.getLabel(i) == blob.label ){
                    blob.found = true;
                    insert = false;

                    if( sendContours ||
                        // check the distance or velocity changed enough
                        ( glm::distance( blob.center, glm::vec2(finder.getCenter(i).x, finder.getCenter(i).y) ) > float(filterDeltaDistance) ) ||
                        (glm::distance( blob.velocity, glm::vec2(finder.getVelocity(i)[0], finder.getVelocity(i)[1]) ) > filterDeltaVelocity ) 
                     ){ // then
                        blob.center.x = finder.getCenter(i).x;
                        blob.center.y = finder.getCenter(i).y;
                        blob.velocity.x = finder.getVelocity(i)[0];
                        blob.velocity.y = finder.getVelocity(i)[1];
                        blob.boundaries = ofxCv::toOf( finder.getBoundingRect(i) );
                        blob.contour = finder.getPolyline( i );
                        blob.update = true;
                    }
                    break;
                }
            }

            if( insert ){
                blobs.emplace_back();
                blobs.back().center.x = finder.getCenter(i).x;
                blobs.back().center.y = finder.getCenter(i).y;
                blobs.back().velocity.x = finder.getVelocity(i)[0];
                blobs.back().velocity.y = finder.getVelocity(i)[1];
                blobs.back().boundaries = ofxCv::toOf( finder.getBoundingRect(i) );
                blobs.back().contour = finder.getPolyline( i );
                blobs.back().label = finder.getLabel(i);
            }
        
        }
    }

    // now delete all the not found and send OSC message for that
    auto it = blobs.begin();
    for ( ; it != blobs.end(); ) {
        if( ! it->found ){
            ofxOscMessage m;
            m.setAddress( "/cvtracker/blobs/delete" );
            m.addIntArg( it->label );
            sender.sendMessage(m, false);
            it = blobs.erase( it );
        }else{
            it++;
        }
    }

    // send all the messages for the blobs to update
    for( auto & blob : blobs ){
        if( blob.update ){
            ofxOscMessage m;

            m.setAddress( "/cvtracker/blobs/update" );

            m.addIntArg( blob.label );

            // normalized coords
            m.addFloatArg( blob.center.x * divideW );
            m.addFloatArg( blob.center.y * divideH );

            // velocities
            m.addFloatArg( blob.velocity.x );
            m.addFloatArg( blob.velocity.y );

            // normalized bounding boundaries
            m.addFloatArg( blob.boundaries.x * divideW);
            m.addFloatArg( blob.boundaries.y * divideH);
            m.addFloatArg( blob.boundaries.width * divideW);
            m.addFloatArg( blob.boundaries.height * divideH);

            if( sendContours ){
                ofPolyline simplified = blob.contour;
                float amt = (simplifyContours.get() > 1.0f) ? 1.0f : simplifyContours.get();
                simplified.simplify( amt );

                int step = ofMap( simplifyContours, 1.0f, 2.0f, 1, 8, true);

                auto & vertices = simplified.getVertices();
                for( size_t i=0; i<vertices.size(); i+=step ){
                    m.addFloatArg( vertices[i].x * divideW );
                    m.addFloatArg( vertices[i].y * divideH );
                }
            }

            sender.sendMessage(m, false);

            blob.update = false;
        }
    }
}
