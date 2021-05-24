//
// Created by lidor on 12/01/2020.
//

#include <Frame.h>
#include <boost/algorithm/string.hpp>
#include <vector>

using std::vector;

Frame::Frame(string msg) {
    vector<string> splitted;
    boost::split(splitted, msg, [](char c) { return c == '\n'; });
    stompCommand = splitted[0];

    int ctr = 1;
    while (!splitted[ctr].empty()) {
        string helper = splitted[ctr];
        vector<string> tmp;
        boost::split(tmp, helper, [](char c) { return c == ':'; });
        headers[tmp[0]] = tmp[1];
        ctr++;
    }
    body = "";
    while (ctr < splitted.size() - 1) {
        ctr++;
        if (ctr != splitted.size() - 1) {
            body += splitted[ctr] + '\n';
        } else {
            body += splitted[ctr];
        }
    }
}

Frame::Frame(string command, unordered_map<string, string> &headers, string body) :
        stompCommand(command), headers(headers), body(body) {

}

string Frame::getCommand() {
    return stompCommand;
}

string Frame::getBody() {
    return body;
}

string Frame::getHeader(string header) {
    return headers[header];
}

string Frame::toString() {
    string result = stompCommand + '\n';
    for (auto pair : headers) {
        result += pair.first + ":" + pair.second + '\n';
    }
    result += '\n';
    if (!body.empty()) {
        result += body + '\n';
    }
    return result;
}

unordered_map<string, string> &Frame::getHeaders() {
    return headers;
}
