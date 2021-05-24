//
// Created by benny on 19/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SERVERHANDLER_H
#define BOOST_ECHO_CLIENT_SERVERHANDLER_H


#include <StompMessagingProtocol.h>

class ServerHandler {
private:
    StompMessagingProtocol protocol;

public:
    ServerHandler(StompMessagingProtocol &protocol);

    void process(Frame& frame);
    bool shouldTerminate();
};


#endif //BOOST_ECHO_CLIENT_SERVERHANDLER_H
