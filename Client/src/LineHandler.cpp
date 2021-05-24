//
// Created by lidor on 12/01/2020.
//

#include <LineHandler.h>
#include <boost/algorithm/string/split.hpp>

using std::string;
using std::vector;

LineHandler::LineHandler(User& user, ConnectionHandler& handler) :
        shouldTerminate_(false), connectionHandler(handler), user(user) {}

void LineHandler::process(string &line) {

    vector<string> splitted;
    boost::split(splitted, line, [](char c) { return c == ' '; });

    string command = "";
    auto* headers = new unordered_map<string,string>();
    string body = "";

    if (splitted[0] == "login") {
        command = "CONNECT";

        vector<string> hostPort;
        boost::split(hostPort, splitted[1], [](char c){return c == ':';});
        string host = hostPort[0];
        short port = atoi(hostPort[1].c_str());
        connectionHandler.setHostPort(host,port);
        if (!user.isLoggedIn() && !connectionHandler.connect()) {
            std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        }
        else {
            user.login();
            string username = splitted[2];
            string passcode = splitted[3];

            user.setUserName(username);
            (*headers)["accept-version"] = "1.2";
            (*headers)["host"] = host + ":" + std::to_string(port);
            (*headers)["login"] = username;
            (*headers)["passcode"] = passcode;
        }
    }
    else if (!user.isLoggedIn()) {
        std::cout<<"user isn't logged-in\n";
    }
    else if (splitted[0] == "join") {
        command = "SUBSCRIBE";
        (*headers)["destination"] = splitted[1];
        (*headers)["id"] = std::to_string(user.incrementSub());
        (*headers)["receipt"] = std::to_string(user.incrementReceipt());
        Frame *receiptFrame = new Frame(command, *headers, body);
        user.connectReceiptToFrame((*headers)["receipt"], *receiptFrame);
    }
    else if (splitted[0] == "exit") {
        command = "UNSUBSCRIBE";
        (*headers)["destination"] = splitted[1];
        (*headers)["id"] = std::to_string(user.incrementSub());
        (*headers)["receipt"] = std::to_string(user.incrementReceipt());
        Frame *receiptFrame = new Frame(command, *headers, body);
        user.connectReceiptToFrame((*headers)["receipt"], *receiptFrame);
    } else if (splitted[0] == "add") {
        command = "SEND";
        (*headers)["destination"] = splitted[1];
        body = user.getUserName() + " has added the Book " + splitted[2];
        Book *book = new Book(splitted[1], user.getUserName(), splitted[2]);
        user.addBook(*book);
    } else if (splitted[0] == "borrow") {
        command = "SEND";
        (*headers)["destination"] = splitted[1];
        body = user.getUserName() + " wish to borrow " + splitted[2];
        user.addToWishlist(splitted[1], splitted[2]);
    } else if (splitted[0] == "return") {
        command = "SEND";
        (*headers)["destination"] = splitted[1];
        body = "Returning " + splitted[2] + " to " + user.getBook(splitted[1], splitted[2])->getOwner();
        user.removeBook(splitted[1],splitted[2]);
    } else if (splitted[0] == "status") {
        command = "SEND";
        (*headers)["destination"] = splitted[1];
        body = "Book status";
    }
    else if (splitted[0] == "logout") {
        command = "DISCONNECT";
        (*headers)["receipt"] = std::to_string(user.incrementReceipt());
        Frame *receiptFrame = new Frame(command, *headers, body);
        user.connectReceiptToFrame((*headers)["receipt"], *receiptFrame);
        terminate();
    }
    else {
        std::cout<<"not a command\n";
    }

    if (user.isLoggedIn()) {
        Frame *msg = new Frame(command, *headers, body);
        string msgToSend = msg->toString();
        string test=msgToSend;
        connectionHandler.sendLine(msgToSend);
        delete msg;
    }
}

bool LineHandler::shouldTerminate() {
    return shouldTerminate_;
}

void LineHandler::terminate() {
    shouldTerminate_ = false;
}
