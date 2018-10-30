
#include "Accelerometer.h"

#define ACCEL_AVG_MAX_FRAMES 200

ofx::clayblocks::Accelerometer::Accelerometer(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	average = 0.0f;

	ax = ay = az = ax_z1 = ay_z1 = az_z1 = 0.0f;
	values.resize( ACCEL_AVG_MAX_FRAMES );
	for( auto & value : values ){
        value.x = 0.0f;
        value.y = 0.0f;
        value.z = 0.0f;
    }

    vindex = 0;

    parameters.setName( "Accelerometer" );
        parameters.add( averageFrames.set("average frames", 121, 2, ACCEL_AVG_MAX_FRAMES ));
        parameters.add( easing.set("easing", 0.0f, 0.0f, 1.0f) );
        parameters.add( xMin.set("x min", -8.0f, 0.0f, -10.0f) );
        parameters.add( xMax.set("x max",  8.0f, 0.0f,  10.0f) );
        parameters.add( yMin.set("y min", -8.0f, 0.0f, -10.0f) );
        parameters.add( yMax.set("y max",  8.0f, 0.0f,  10.0f) );
        parameters.add( zMin.set("z min", -8.0f, 0.0f, -10.0f) );
        parameters.add( zMax.set("z max",  8.0f, 0.0f,  10.0f) );

}

ofx::clayblocks::Accelerometer::~Accelerometer(){
	if(isThreadRunning()){
        stopThread();
    }
    ofSleepMillis(50);
}

void ofx::clayblocks::Accelerometer::start( int port, std::string oscDevice ){

    deviceName = oscDevice;
    if(deviceName.substr(0, 1)!="/"){
        deviceName = "/" + deviceName;
    }

    xname = deviceName + "/ax";
    yname = deviceName + "/ay";
    zname = deviceName + "/az";

	std::cout << "[ofx::clayblocks::Accelerometer] listening for phone Accelerometer OSC messages on port "<<port<<"\n";
    oscReceiver.setup( port );

    parameters.setName( parameters.getName() + " p" + ofToString( port ) );

	ofSleepMillis(50);
	startThread();
}

void ofx::clayblocks::Accelerometer::threadedFunction() {

	while( isThreadRunning() ) {

        while( oscReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            oscReceiver.getNextMessage(m);

            if(m.getAddress() == xname ){
                ax = m.getArgAsFloat(0);
                ax = ofMap( ax, xMin, xMax, -1.0f, 1.0f, true );
            }

            if(m.getAddress() == yname ){
                ay = m.getArgAsFloat(0);
                ay = ofMap( ay, yMin, yMax, -1.0f, 1.0f, true );
            }

            if(m.getAddress() == zname){
                az = m.getArgAsFloat(0);
                az = ofMap( az, zMin, zMax, -1.0f, 1.0f, true );
            }
        }

        // easing -----------------------------------------------
        ax = ax_z1 * easing + ax*(1.0f-easing);
        ay = ay_z1 * easing + ay*(1.0f-easing);
        az = az_z1 * easing + az*(1.0f-easing);
        ax_z1 = ax;
        ay_z1 = ay;
        az_z1 = az;
        x = ax;
        y = ay;
        z = az;


        // --------------- calculate average --------------------
        values[vindex] = glm::vec3( ax, ay, az );
        vindex++;
        if( vindex>=values.size() ){ vindex=0; }


        float dsum = 0.0f;

        int i = vindex;
        int i_z1 = i-1;
        if( i_z1 < 0 ){ i_z1 += values.size(); }

        for( int n=0; n<averageFrames; ++n){

            float delta = abs( values[i].x - values[i_z1].x );
            delta += abs( values[i].y - values[i_z1].y );
            delta += abs( values[i].z - values[i_z1].z );

            dsum += delta;


            i_z1 = i;
            i++;
            i = ( i==ACCEL_AVG_MAX_FRAMES ) ? 0 : i;

        }

        dsum /= averageFrames;
        dsum = (dsum>1.0f) ? 1.0f : dsum;
        average = dsum;

		ofSleepMillis(5);
	}
}

void ofx::clayblocks::Accelerometer::draw( int x, int y ) {
	ofPushMatrix();
    ofPushStyle();
	ofTranslate( x, y );
    {
		const int h = 14;
		const int w = 200;
        static const float scale = 0.5f;
        static const float mscale = 1.0f;
        const int gx = 30;
        const int gy = 10;
        ofNoFill();
        ofDrawBitmapString( "x", gx-30, gy   +14 );
        ofDrawBitmapString( "y", gx-30, gy+h+14 );
        ofDrawBitmapString( "z", gx-30, gy+h*2+14 );
        ofDrawBitmapString( "average", gx-30, gy+h*3+14 );
        ofDrawRectangle( gx, gy,    	w, h );
        ofDrawRectangle( gx, gy+h, 	    w, h );
        ofDrawRectangle( gx, gy+h*2,	w, h );
        ofDrawLine( gx+w*0.5, gy,    	gx+w*0.5, gy    +h );
        ofDrawLine( gx+w*0.5, gy+h, 	gx+w*0.5, gy+h  +h );
        ofDrawLine( gx+w*0.5, gy+h*2,   gx+w*0.5, gy+h*2+h );
        ofFill();
        ofDrawRectangle( gx+w*0.5f, gy,    	w*this->x*scale, h ); // acc * 0.1f (0-10 range) * 0.5f
        ofDrawRectangle( gx+w*0.5f, gy+h, 	w*this->y*scale, h );
        ofDrawRectangle( gx+w*0.5f, gy+h*2, 	w*this->z*scale, h );
        ofDrawRectangle( gx, gy+h*3, 	w*this->average*mscale, h );
    }
    ofPopStyle();
	ofPopMatrix();
}

void ofx::clayblocks::Accelerometer::stop(){
	stopThread();
}

ofParameterGroup & ofx::clayblocks::Accelerometer::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

ofx::clayblocks::Accelerometer::Accelerometer( const Accelerometer & other){
	x.store( other.x );
	y.store( other.y );
	z.store( other.z );
	average.store( other.average );
	values = other.values;
	ax = other.ax;
	ay = other.ay;
	az = other.az;
	ax_z1 = other.ax_z1;
	ay_z1 = other.ay_z1;
	az_z1 = other.az_z1;
	vindex = other.vindex;

    parameters.setName( "Accelerometer" );
        parameters.add( averageFrames.set("average frames", other.averageFrames, 2, ACCEL_AVG_MAX_FRAMES ));
        parameters.add( easing.set("easing", other.easing, 0.0f, 1.0f) );
        parameters.add( xMin.set("x min", other.xMin, 0.0f, -10.0f) );
        parameters.add( xMax.set("x max", other.xMax, 0.0f,  10.0f) );
        parameters.add( yMin.set("y min", other.yMin, 0.0f, -10.0f) );
        parameters.add( yMax.set("y max", other.yMax, 0.0f,  10.0f) );
        parameters.add( zMin.set("z min", other.zMin, 0.0f, -10.0f) );
        parameters.add( zMax.set("z max", other.zMax, 0.0f,  10.0f) );
}

ofx::clayblocks::Accelerometer::Accelerometer( Accelerometer && other){
	x.store( other.x );
	y.store( other.y );
	z.store( other.z );
	average.store( other.average );
	values = other.values;
	ax = other.ax;
	ay = other.ay;
	az = other.az;
	ax_z1 = other.ax_z1;
	ay_z1 = other.ay_z1;
	az_z1 = other.az_z1;
	vindex = other.vindex;

    parameters.setName( "Accelerometer" );
        parameters.add( averageFrames.set("average frames", other.averageFrames, 2, ACCEL_AVG_MAX_FRAMES ));
        parameters.add( easing.set("easing", other.easing, 0.0f, 1.0f) );
        parameters.add( xMin.set("x min", other.xMin, 0.0f, -10.0f) );
        parameters.add( xMax.set("x max", other.xMax, 0.0f,  10.0f) );
        parameters.add( yMin.set("y min", other.yMin, 0.0f, -10.0f) );
        parameters.add( yMax.set("y max", other.yMax, 0.0f,  10.0f) );
        parameters.add( zMin.set("z min", other.zMin, 0.0f, -10.0f) );
        parameters.add( zMax.set("z max", other.zMax, 0.0f,  10.0f) );
}

ofx::clayblocks::Accelerometer & ofx::clayblocks::Accelerometer::operator= (const Accelerometer& other){
	x.store( other.x );
	y.store( other.y );
	z.store( other.z );
	average.store( other.average );
	values = other.values;
	ax = other.ax;
	ay = other.ay;
	az = other.az;
	ax_z1 = other.ax_z1;
	ay_z1 = other.ay_z1;
	az_z1 = other.az_z1;
	vindex = other.vindex;

    parameters.setName( "Accelerometer" );
        parameters.add( averageFrames.set("average frames", other.averageFrames, 2, ACCEL_AVG_MAX_FRAMES ));
        parameters.add( easing.set("easing", other.easing, 0.0f, 1.0f) );
        parameters.add( xMin.set("x min", other.xMin, 0.0f, -10.0f) );
        parameters.add( xMax.set("x max", other.xMax, 0.0f,  10.0f) );
        parameters.add( yMin.set("y min", other.yMin, 0.0f, -10.0f) );
        parameters.add( yMax.set("y max", other.yMax, 0.0f,  10.0f) );
        parameters.add( zMin.set("z min", other.zMin, 0.0f, -10.0f) );
        parameters.add( zMax.set("z max", other.zMax, 0.0f,  10.0f) );

	return *this;
}

ofx::clayblocks::Accelerometer & ofx::clayblocks::Accelerometer::operator= (Accelerometer&& other) noexcept {
	x.store( other.x );
	y.store( other.y );
	z.store( other.z );
	average.store( other.average );
	values = other.values;
	ax = other.ax;
	ay = other.ay;
	az = other.az;
	ax_z1 = other.ax_z1;
	ay_z1 = other.ay_z1;
	az_z1 = other.az_z1;
	vindex = other.vindex;

    parameters.setName( "Accelerometer" );
        parameters.add( averageFrames.set("average frames", other.averageFrames, 2, ACCEL_AVG_MAX_FRAMES ));
        parameters.add( easing.set("easing", other.easing, 0.0f, 1.0f) );
        parameters.add( xMin.set("x min", other.xMin, 0.0f, -10.0f) );
        parameters.add( xMax.set("x max", other.xMax, 0.0f,  10.0f) );
        parameters.add( yMin.set("y min", other.yMin, 0.0f, -10.0f) );
        parameters.add( yMax.set("y max", other.yMax, 0.0f,  10.0f) );
        parameters.add( zMin.set("z min", other.zMin, 0.0f, -10.0f) );
        parameters.add( zMax.set("z max", other.zMax, 0.0f,  10.0f) );

	return *this;
}
