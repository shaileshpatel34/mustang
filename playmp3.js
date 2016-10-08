if ( session.ready( ) ) {
        session.answer( );
        session.streamFile( "shout://s3.amazonaws.com/plivocloud/Trumpet.mp3" , "");
        if ( session.ready( ) ) {
                session.hangup( );
        }
}
