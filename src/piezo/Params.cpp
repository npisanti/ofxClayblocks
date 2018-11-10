
#include "Params.h"

ofx::clayblocks::piezo::Params::Params() {
    parameters.setName( "piezo sensor" );
    parameters.add( active.set("active", true ) );
    parameters.add( thresholdLow.set("threshold low", 0, 0, 1023 ) );
    parameters.add( thresholdHigh.set("threshold high", 1023, 0, 1023 ) );
}

ofParameterGroup & ofx::clayblocks::piezo::Params::label( std::string name ){
    parameters.setName( name );
    return parameters;
}
