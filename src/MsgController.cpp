//
//  MsgController.cpp
//  MsgController
//
//  Created by Nicole Yi Messier
//

#include "MsgController.h"

MsgController::MsgController()
{
    
}

MsgController::~MsgController()
{
}

void MsgController::setup( const bool use_hb, string heartbeat_ip, int heartbeat_port, const bool use_sender, int rx_port,
    int node_server_port_tx,
                           string server_ip)
{
    //! setup receiver
    receiver = make_unique<ofxOscReceiver>();
    receiver->setup( rx_port );

    //! setup sender
    mUseSender = use_sender;
    if( mUseSender ) {
        sender = make_unique<ofxOscSender>();
        sender->setup( server_ip, node_server_port_tx );
    }

    //! setup heartbeat
    mUseHb = use_hb;
    if( mUseHb ) {
        //! setup heartbeat
        hb.setup( heartbeat_ip, APP_ID, heartbeat_port );
        hb.start();
        hb.resetCountdown();
    }
}

void MsgController::update( float dt )
{
    receiveMessages();

    if( mUseHb )
        hb.update( dt );
}

void MsgController::draw()
{
}

#pragma mark SENDER

void MsgController::sendAnalytics( string content_id )
{
    ofxOscMessage m;
    m.setAddress( mAnalytics );
    string data = serializeAnalytics( content_id );
    m.addStringArg( data );
    sender->sendMessage( m, false );
}

string MsgController::serializeAnalytics( string content_id )
{
    ofJson analytics;
    analytics["content_id"] = content_id;

    ofLogNotice( "MsgController::serializeToJson" ) << analytics.dump();

    return analytics.dump();
}

#pragma mark RECEIVER MESSAGES

void MsgController::receiveMessages()
{

    // check for waiting messages
    while( receiver->hasWaitingMessages() ) {

        // get the next message
        ofxOscMessage m;
        receiver->getNextMessage( m );
        decodeOsc( m );
    }
}

void MsgController::decodeOsc( const ofxOscMessage &msg )
{

    if( msg.getAddress() == mHeartbeat ) {
        mHbMsg = "Received heartbeat at " + ofToString( ofGetElapsedTimef() );
        ofLogNotice() << "Received heartbeat at " << mHbMsg;
    }
    else if( msg.getAddress() == mRestart )
    {
        
        ofLogNotice() << "Received restart at " << mRestart;
        ofExit(); 
    }
    else 
        ofLogError( "MsgController::decodeOSC" ) << msg.getAddress() << " does not match any stored addresses!";
}


string MsgController::getParsedAddress( string full )
{
    //! check
    vector<string> split = ofSplitString( full, "/" );
    string         new_adress = "";

    for( int i = 0; i >= split.size() - 1; ++i ) {
        new_adress += split[i];
        new_adress += "/";
    }

    return new_adress;
}