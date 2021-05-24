//
// Created by lidor on 12/01/2020.
//

#include <StompMessagingProtocol.h>
#include <string>
#include <boost/algorithm/string/split.hpp>


using std::string;
using std::cout;

StompMessagingProtocol::StompMessagingProtocol(User& user,ConnectionHandler& handler):
    user(user),connectionHandler(handler) {}

void StompMessagingProtocol::process(Frame &frame) {
    string command(frame.getCommand());
    string body = frame.getBody();

    if (command == "CONNECTED") {
        cout << "Login successful\n";
    }
    else if (command == "MESSAGE") {

        cout << frame.getHeader("destination") << ":" << body<<'\n';

        if (body.find("wish to borrow") != -1) {
            string borrower = body.substr(0,body.find(" wish"));
            string genre = frame.getHeader("destination");
            string bookName = body.substr(body.find("borrow")+7);
            if (user.getUserName() != borrower && user.bookExist(genre,bookName)) {
                auto headers = unordered_map<string,string>();
                headers["destination"] = genre;
                string lender = user.getUserName();
                string msg = Frame("SEND", headers, lender + " has " + bookName).toString();
                connectionHandler.sendLine(msg);
            }
        }
        else if (body.find("has") != -1 && body.find("added") == -1) {
            string genre = frame.getHeader("destination");
            string bookName = body.substr(body.find("has")+4);
            if (!user.getWantedBook(genre,bookName).empty()) {
                string owner = body.substr(0,body.find("has"));
                Book* book = new Book(genre,owner,bookName);
                user.addBook(*book);
                auto headers = unordered_map<string,string>();
                headers["destination"] = genre;
                string msg = Frame("SEND",headers,"Taking " + bookName + " from " + owner).toString();
                connectionHandler.sendLine(msg);
            }
        }
        else if (body.find("Taking") != -1) {
            if (body.substr(body.find("from")+5) == user.getUserName()) {
                string genre = frame.getHeader("destination");
                string bookName = body.substr(7,body.find(" from "+user.getUserName())-7);
                user.getBook(genre,bookName)->setAvailable(false);
            }
        }
        else if (body.find("Returning") != -1) {
            string owner = body.substr(body.find("to") + 3);
            if (user.getUserName() == owner) {
                string genre = frame.getHeader("destination");
                string bookName = body.substr(10, body.find(" to " + owner) - 10);
                user.getBook(genre,bookName)->setAvailable(true);
            }
        }
        else if (body.find("status") != -1) {
            string genre = frame.getHeader("destination");
            auto genreLibrary = user.getGenreLibrary(genre);
            string resultBody = user.getUserName()+":";
            for (Book& book : genreLibrary) {
                if (book.isAvailable()) {
                    resultBody += book.getName() + ",";
                }
            }
            auto headers = unordered_map<string,string>();
            headers["destination"] = genre;
            string msg = Frame("SEND", headers, resultBody.substr(0,resultBody.size()-1)).toString();
            connectionHandler.sendLine(msg);
        }
    }
    else if (command == "RECEIPT") {
        Frame linkedFrame = user.getFrameFromReceipt(frame.getHeader("receipt-id"));
        string linkedCommand = linkedFrame.getCommand();
        auto linkedHeaders = linkedFrame.getHeaders();
        if (linkedCommand == "SUBSCRIBE") {
            user.setGenreSubId(linkedHeaders["destination"], linkedHeaders["id"]);
            cout << "Joined club " + linkedHeaders["destination"] + '\n';
        }
        else if (linkedCommand == "UNSUBSCRIBE") {
            string genre = linkedHeaders["destination"];
            user.getGenreSubId().erase(genre);
            cout << "Exited club " + genre +'\n';
        }
        else if (linkedCommand == "DISCONNECT") {
            terminate();
        }
    }
    else if (command == "ERROR") {
        cout <<"ERROR:\n" << body + '\n';
    }
}

bool StompMessagingProtocol::shouldTerminate() {
    return shouldTerminate_;
}

void StompMessagingProtocol::terminate() {
    shouldTerminate_ = true;
}
