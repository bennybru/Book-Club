//
// Created by lidor on 12/01/2020.
//

#ifndef FRAME_H_
#define FRAME_H_

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class Frame {
private:
    string stompCommand;
    unordered_map<string,string> headers;
    string body;

public:
    Frame(string msg);
    Frame(string command,unordered_map<string,string>& headers,string body);

    string toString();

    string getCommand();
    string getHeader(string header);
    unordered_map<string,string>& getHeaders();
    string getBody();
};


#endif //BOOST_ECHO_CLIENT_FRAME_H
