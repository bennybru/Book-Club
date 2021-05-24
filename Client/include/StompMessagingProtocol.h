//
// Created by lidor on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H

#include "Frame.h"
#include "User.h"
#include "ConnectionHandler.h"

class StompMessagingProtocol {

private:
    User& user;
    ConnectionHandler& connectionHandler;
    bool shouldTerminate_;

public:
    StompMessagingProtocol(User& user,ConnectionHandler& handler);
    void process(Frame& frame);
    bool shouldTerminate();
    void terminate();
};


#endif //BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
