//
// Created by benny on 19/01/2020.
//

#include <ServerHandler.h>

ServerHandler::ServerHandler(StompMessagingProtocol &protocol) : protocol(protocol) {}

void ServerHandler::process(Frame &frame) {
    protocol.process(frame);
}

bool ServerHandler::shouldTerminate() {
    return protocol.shouldTerminate();
}