//
// Created by lidor on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_LINEHANDLER_H
#define BOOST_ECHO_CLIENT_LINEHANDLER_H

#include <ConnectionHandler.h>
#include <User.h>

using std::string;

class LineHandler {
private:
    bool shouldTerminate_;
    ConnectionHandler& connectionHandler;
    User& user;

public:
    LineHandler(User& user, ConnectionHandler& handler);
    void process(string &line);
    bool shouldTerminate();
    void terminate();
};


#endif //BOOST_ECHO_CLIENT_LINEHANDLER_H
