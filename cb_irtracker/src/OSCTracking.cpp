
#include "OSCTracking.h"

#define BUFFERW 160
#define BUFFERH 120

np::helper::OSCTracking::OSCTracking(){
    blobs.reserve(128); // more than enough
    blobs.clear();


    bTakeBackground = true;
    finder.setThreshold(127);

    buffersize = BUFFERW*BUFFERH;
    buffer = new char[buffersize];

    parameters.setName( "cvtracker");
        parameters.add( doBackgroundSubtraction.set("background subtraction", false) );
            //doBackgroundSubtraction.addListener( this, &ofApp::onBGSubtractionToggle );
        parameters.add( denoise.set("denoise", false) );
        parameters.add( low.set("threshold low", 0, 0, 255) );
        parameters.add( high.set("threshold high", 255, 0, 255) );
        parameters.add( minArea.set("area min", 20, 1, 25000) );
        parameters.add( maxArea.set("area max", 20000, 1, 100000) );;
        parameters.add( persistence.set("persistence", 15, 1, 100) );
        parameters.add( maxDistance.set("max distance", 32, 1, 100) );
        parameters.add( distSensitivity.set("distance sensitivity", 10, 0, 80) );
        parameters.add( veloSensitivity.set("velo sensitivity", 0.01f, 0.00001f, 1.0f) );
        parameters.add( sendContours.set( "send contours", false ) );
        parameters.add( simplifyContours.set( "simplify contours", 0.6f, 0.0f, 2.0f ) );
        parameters.add( minX.set("min X", 0.0f, 0.0f, 1.0f) );
        parameters.add( maxX.set("max X", 1.0f, 0.0f, 1.0f) );
        parameters.add( minY.set("min Y", 0.0f, 0.0f, 1.0f) );
        parameters.add( maxY.set("max Y", 1.0f, 0.0f, 1.0f) );
        parameters.add( sendImage.set("send image", 0, 0, 2) );

    network.setName("network");
        network.add( ip.set("client ip", "localhost") );

        ports.setName("ports");
            ports.add( oscPort.set( "osc send", 12345, 0, 15000) );
            ports.add( syncSendPort.set("sync send", 4243, 0, 15000) );
            ports.add( syncReceivePort.set("sync receive", 4244, 0, 15000) );
        network.add( ports );
}

np::helper::OSCTracking::~OSCTracking(){
    delete [] buffer;
}

void np::helper::OSCTracking::setup( int width, int height, const cv::Mat & toImitate ){
    this->width = width;
    this->height = height;

    sender.setup( ip, oscPort );
    synchronizer.setup( parameters, syncReceivePort, ip, syncSendPort );

    std::cout<<"[cvtracker] sending tracking OSC to "<<ip<<" on port "<<oscPort<<"\n";
    std::cout<<"[cvtracker] syncing parameters to "<<ip<<" with ports "<<syncSendPort<<" (send) and "<<syncReceivePort<<" (receive) \n";

    divideW = 1.0f / float(width);
    divideH = 1.0f / float(height);

    ofxCv::imitate( tLow, toImitate );
    ofxCv::imitate( tHigh, toImitate );
    ofxCv::imitate( thresh, toImitate );
    ofxCv::imitate( background, toImitate );

}

void np::helper::OSCTracking::sync(){
    synchronizer.update();
}

void np::helper::OSCTracking::update( cv::Mat & frame ){

    if( bTakeBackground ){
        background = frame;
        bTakeBackground = false;
    }

    if( doBackgroundSubtraction ){
        cv::Mat subtracted;
        cv::subtract( frame, background, subtracted );
        frame = subtracted;
    }

    if(denoise){
        cv::medianBlur( frame, frame, 3 );
    }

    cv::threshold( frame, tLow,   low, 255,  0 );
    cv::threshold( frame, tHigh,  high, 255, 1 );
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

    float x0 = minX*width;
    float x1 = maxX*width;
    float y0 = minY*height;
    float y1 = maxY*height;

    for ( auto & blob : blobs ){ blob.found = false; }


    for( size_t i=0; i<finder.size(); ++i ){

        if( finder.getCenter(i).x > x0 && finder.getCenter(i).x < x1 && finder.getCenter(i).y > y0 && finder.getCenter(i).y < y1 ){

            bool insert = true;

            for( auto & blob : blobs ){

                if( finder.getLabel(i) == blob.label ){
                    blob.found = true;
                    insert = false;

                    if( sendContours ||
                        // check the distance or velocity changed enough
                        ( glm::distance( blob.center, glm::vec2(finder.getCenter(i).x, finder.getCenter(i).y) ) > float(distSensitivity) ) ||
                        (glm::distance( blob.velocity, glm::vec2(finder.getVelocity(i)[0], finder.getVelocity(i)[1]) ) > veloSensitivity )
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

void np::helper::OSCTracking::onBGSubtractionToggle( bool & value ){
    if( value ){
        bTakeBackground = true;
    }
}
