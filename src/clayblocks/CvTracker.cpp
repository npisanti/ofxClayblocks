
#include "CvTracker.h"

#define BUFFERW 160
#define BUFFERH 120

ofx::clayblocks::CvTracker::Blob::Blob(){
    label = 0;
    position.x = -1.0f;
    position.y = -1.0f;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    boundaries = ofRectangle( 0, 0, 0, 0 );
}

ofx::clayblocks::CvTracker::CvTracker(){

    parameters.setName( "CvTracker" );

    tracker.setName( "cvtracker");
        tracker.add( doBackgroundSubtraction.set("background subtraction", false) );
        tracker.add( denoise.set("denoise", false) );
        tracker.add( low.set("threshold low", 0, 0, 255) );
        tracker.add( high.set("threshold high", 255, 0, 255) );
        tracker.add( minArea.set("area min", 20, 1, 5000) );
        tracker.add( maxArea.set("area max", 15000, 1, 50000) );;
        tracker.add( persistence.set("persistence", 15, 1, 100) );
        tracker.add( maxDistance.set("max distance", 32, 1, 100) );
        tracker.add( distSensitivity.set("distance sensitivity", 10, 0, 80 ) );
        tracker.add( veloSensitivity.set("velo sensitivity", 0.01f, 0.00001f, 1.0f) );
        tracker.add( sendContours.set( "send contours", false ) );
        tracker.add( simplifyContours.set( "simplify contours", 0.6f, 0.0f, 2.0f ) );
        tracker.add( minX.set("min X", 0.0f, 0.0f, 1.0f) );
        tracker.add( maxX.set("max X", 1.0f, 0.0f, 1.0f) );
        tracker.add( minY.set("min Y", 0.0f, 0.0f, 1.0f) );
        tracker.add( maxY.set("max Y", 1.0f, 0.0f, 1.0f) );
        tracker.add( sendImage.set("send image", 0, 0, 2) );
    parameters.add( tracker );

    simulation.setName( "simulation" );
        simulation.add( simulate.set("simulate", false) );
        simulation.add( simulatedBlobs.set("simulated blobs", 1, 0, 10) );
        simulation.add( speed.set("speed", 0.035f, 0.0f, 0.300f) );
    t0 = 0.0f;
    parameters.add( simulation );

    simulate.addListener( this, &ofx::clayblocks::CvTracker::onSimulate );

    bRecordAverage = false;
    recorded.reserve( 128 );

    active = false;
}


void ofx::clayblocks::CvTracker::mapToSurface( int w, int h ){

    mapping.setName( "mapping" );
    mapping.add( active.set("active", true) );
    input.setName( "input" );
        input.add( topX0.set("top x0", 0.0f, 0.0f, 1.0f) );
        input.add( topX1.set("top x1", 1.0f, 0.0f, 1.0f) );
        input.add( topY.set("top y", 0.0f, 0.0f, 1.0f) );  
        input.add( bottomX0.set("bottom x0", 0.0f, 0.0f, 1.0f) );
        input.add( bottomX1.set("bottom x1", 1.0f, 0.0f, 1.0f) );
        input.add( bottomY.set("bottom y", 1.0f, 0.0f, 1.0f) );
    mapping.add( input );
    
    output.setName( "output" );
        output.add( outX.set("x", 0, 0, w) );
        output.add( outY.set("y", 0, 0, h) );
        output.add( outW.set("w", w, 0, w) );
        output.add( outH.set("h", h, 0, h) );
    mapping.add( output );

    parameters.add( mapping );

}

void ofx::clayblocks::CvTracker::onSimulate( bool & value ){
    if( ! simulate ){
        blobs.clear();
    }
}

void ofx::clayblocks::CvTracker::setup( int oscPort, std::string serverIp, int syncReceivePort, int syncSendPort ){

    blobs.reserve( 128 );

    std::cout<<"[CvTracker client] listening for tracking OSC on port "<<oscPort<<"\n";
    std::cout<<"[CvTracker client] syncing parameters to "<<serverIp<<" with ports "<<syncSendPort<<" (send) and "<<syncReceivePort<<" (receive) \n";

    oscReceiver.setup( oscPort );

    sync.setup( tracker, syncReceivePort, serverIp, syncSendPort );

    receivedImage.allocate( BUFFERW, BUFFERH, OF_IMAGE_COLOR );
    bImageReceived = false;
}


void ofx::clayblocks::CvTracker::update(){

    if( ! simulate ){
        sync.update();

        while( oscReceiver.hasWaitingMessages() ){
            ofxOscMessage m;
            oscReceiver.getNextMessage(m);

            // getting blobs
            if( m.getAddress() == "/cvtracker/blobs/update" ){

                bool insert = true;

                unsigned int label = m.getArgAsInt32( 0 );
                for(auto & blob : blobs ){
                    if( blob.label == label ){
                        updateBlob( blob, m );
                        insert = false;
                    }
                }

                if( insert ){
                    blobs.emplace_back();
                    auto & blob = blobs.back();
                    blob.label = label;
                    updateBlob( blob, m );
                }

            } else if( m.getAddress() == "/cvtracker/blobs/delete" ){

                unsigned int label = m.getArgAsInt32( 0 );

                for ( auto it = blobs.begin(); it != blobs.end(); it++ ) {
                    if( it->label == label ){
                        blobs.erase( it );
                        break;
                    }
                }

            } else if(m.getAddress() == "/cvtracker/image"){ // getting image

                char* data = m.getArgAsBlob(0).getData();
                auto & pixels = receivedImage.getPixels();

                for( int x=0; x<BUFFERW; ++x){
                    for( int y=0; y<BUFFERH; ++y){
                        pixels.setColor( x, y, ofColor( data[x+y*BUFFERW]) );
                    }
                }
                receivedImage.update();
                bImageReceived = true;
            }
        }
    }else{
        blobs.resize( simulatedBlobs );
        static const float offset = 1.37f;
        float t1 = ofGetElapsedTimef();
        for( size_t i=0; i<blobs.size(); ++i ){
            blobs[i].position.x = ofNoise( i*4*offset, t1*speed );
            blobs[i].position.y = ofNoise( (i*4 +1)*offset, t1*speed );
            blobs[i].boundaries.width = 0.05f + ofNoise( (i*4 +1)*offset, t1*speed ) * 0.05f;
            blobs[i].boundaries.height = 0.05f + ofNoise( (i*4 +1)*offset, t1*speed ) * 0.05f;
            blobs[i].boundaries.x = blobs[i].position.x - blobs[i].boundaries.width * 0.5f;
            blobs[i].boundaries.y = blobs[i].position.y - blobs[i].boundaries.height * 0.5f;

            blobs[i].velocity.x = blobs[i].position.x - ofNoise( i*4*offset, t0*speed );
            blobs[i].velocity.y = blobs[i].position.y - ofNoise( (i*4 +1)*offset, t0*speed );
            blobs[i].velocity.x *= 1500.0f;
            blobs[i].velocity.y *= 1500.0f;
            blobs[i].label = i;
        }
        t0 = t1;
    }

    if( bRecordAverage ){
        glm::vec2 sum( 0, 0 );
        for( auto & blob : blobs ){
            sum.x += blob.position.x;
            sum.y += blob.position.y;
        }
        sum.x /= float( blobs.size() );
        sum.y /= float( blobs.size() );
        recorded.push_back( sum );
    }
}

inline void ofx::clayblocks::CvTracker::doMapping( float & x, float & y ){

    float mapY = ofMap( y, topY, bottomY, 0.0f, 1.0f, true );

    float tx = ofMap( x, topX0, topX1, 0.0f, 1.0f, true );
    float bx = ofMap( x, bottomX0, bottomX1, 0.0f, 1.0f, true );

    float mapX =  tx*(1.0f-mapY) + bx * mapY;

    x = outX + mapX * outW;
    y = outY + mapY * outH;

}

void ofx::clayblocks::CvTracker::updateBlob( Blob & blob, ofxOscMessage & m ){

    blob.position.x = m.getArgAsFloat( 1 );
    blob.position.y = m.getArgAsFloat( 2 );
    blob.velocity.x = m.getArgAsFloat( 3 );
    blob.velocity.y = m.getArgAsFloat( 4 );
    blob.boundaries.x = m.getArgAsFloat( 5 );
    blob.boundaries.y = m.getArgAsFloat( 6 );
    blob.boundaries.width = m.getArgAsFloat( 7 );
    blob.boundaries.height = m.getArgAsFloat( 8 );

    blob.contour.clear();
    for(size_t i = 9; i < m.getNumArgs(); i+=2) {
        blob.contour.addVertex( m.getArgAsFloat(i), m.getArgAsFloat(i+1) );
    }


    if( active ){
        doMapping( blob.position.x, blob.position.y );
        float blx = blob.boundaries.x;
        float bly =  blob.boundaries.y;
        blx += blob.boundaries.width;
        bly += blob.boundaries.height; 
        doMapping( blx, bly );
        doMapping( blob.boundaries.x, blob.boundaries.y );
        blob.boundaries.width = blx - blob.boundaries.x;
        blob.boundaries.height = bly - blob.boundaries.y;

        auto & vertices = blob.contour.getVertices();
        for ( size_t v=0; v<vertices.size(); ++v ){
            doMapping( vertices[v].x, vertices[v].y );
        }
    }

}

void ofx::clayblocks::CvTracker::draw( int x, int y, int w, int h ){

    ofPushMatrix();
    ofPushStyle();
        ofTranslate(x, y );

        if( bImageReceived && sendImage>0 ){
            ofSetColor(255);
            receivedImage.draw( 0, 0, w, h );
        }else{
            ofFill();
            ofSetColor(0);
            ofDrawRectangle( 0, 0, w, h );
            ofSetColor( 255, 0, 0 );
            if( sendImage>0 ) ofDrawBitmapString( "waiting for image blob", x+50, y+50 );
        }

        ofSetColor( 255, 0, 0 );
        
        if( active ){ 

            ofSetColor( 0, 255, 255, 100 );

            for( auto & blob : blobs ){
                float pctx = float(blob.position.x - outX) / float(outW); 
                float pcty = float(blob.position.y - outY) / float(outH);

                float tx = (topX0 * (1.0-pctx) + topX1 * pctx) * w;
                float bx = (bottomX0 * (1.0-pctx) + bottomX1 * pctx) * w;

                float cy = (topY * (1.0-pcty) + bottomY * pcty) * h;
                float cx = tx * (1.0-pcty) + bx * pcty;

                ofFill();
                ofDrawCircle( cx, cy, 7 );
            }

            ofDrawLine( topX0*w, topY*h, topX1*w, topY*h );
            ofDrawLine( topX0*w, topY*h, bottomX0*w, bottomY*h );
            ofDrawLine( topX1*w, topY*h, bottomX1*w, bottomY*h );
            ofDrawLine( bottomX0*w, bottomY*h, bottomX1*w, bottomY*h );
            
        }else{

            ofPushMatrix();
            ofScale( w, h );

            if( sendContours ){
                for( auto & blob : blobs ){
                    ofFill();
                    ofDrawCircle( blob.position.x, blob.position.y, 0.005f );
                    ofNoFill();

                    auto & vertices = blob.contour.getVertices();
                    ofBeginShape();
                    for(size_t i=0; i<vertices.size(); ++i){
                        ofVertex( vertices[i].x, vertices[i].y);
                    }
                    ofEndShape( true );
                    ofDrawBitmapString( ofToString(vertices.size()), blob.position.x, blob.position.y);

                    auto dest = blob.position + blob.velocity * 0.004f;
                    ofDrawLine( blob.position.x, blob.position.y, dest.x, dest.y );
                }
            }else{
                for( auto & blob : blobs ){
                    ofFill();
                    ofDrawCircle( blob.position.x, blob.position.y, 0.005f );
                    ofNoFill();
                    ofDrawRectangle( blob.boundaries.x, blob.boundaries.y,
                                    blob.boundaries.width, blob.boundaries.height );
                    auto dest = blob.position + blob.velocity * 0.004f;
                    ofDrawLine( blob.position.x, blob.position.y, dest.x, dest.y );
                }
            }
            ofPopMatrix();            
        }

        ofSetColor( 150, 40, 40 );
        ofDrawLine( w*minX, 0, w*minX, h );
        ofDrawLine( w*maxX, 0, w*maxX, h );
        ofDrawLine( 0, h*minY, w, h*minY );
        ofDrawLine( 0, h*maxY, w, h*maxY );

        ofSetColor(255);
        ofNoFill();
        ofDrawRectangle( 0, 0, w, h );

    ofPopStyle();
    ofPopMatrix();
}

void ofx::clayblocks::CvTracker::startRecordingAverage(){
    recorded.clear();
    bRecordAverage = true;
}

void ofx::clayblocks::CvTracker::stopRecordingAverage(){
    average.x = 0.0f;
    average.y = 0.0f;

    for( auto & point : recorded ){
        average.x += point.x;
        average.y += point.y;
    }

    average.x /= float( recorded.size() );
    average.y /= float( recorded.size() );

    recorded.clear();
    bRecordAverage = false;
}
