//
//  MsgController.hpp
//  MsgController
//
//  Created by Nicole Yi Messier
//

#pragma once
#include "ofMain.h"
#include "ofxHeartbeat.h"
#include "ofxOscReceiver.h"

class MsgController {
  public:
    MsgController();
    ~MsgController();

    void setup( const bool use_hb, string heartbeat_ip, int heartbeat_port, const bool use_sender, int rx_port, int node_server_port_tx,
        string server_ip );

    void update( float dt );
    void draw();

    void          sendOscMsg();
    void          sendAnalytics( string content_id );
    string        getHbMsg() const { return mHbMsg; }
    string        getCurrentStateString() const;
    string        serializeAnalytics( string content_id );
    static string getParsedAddress( string full );

    //! setters
    void setAppId( string id ) { appId = id; }

  private:
    unique_ptr<ofxOscSender>   sender;
    unique_ptr<ofxOscReceiver> receiver;
    ofxHeartbeat               hb;

    //! addresses
    string mState{ "/state" };
    string mHeartbeat{ "/heart" };
    string mAnalytics{ "/analytics" };
    string mRestart{ "/restart" };
    string mHbMsg{};
    string appId{ "APP" };
    bool   mUseHb{ false };
    bool   mUseSender{ false };

    void receiveMessages();
    void decodeOsc( const ofxOscMessage &msg );
};