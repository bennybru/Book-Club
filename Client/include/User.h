//
// Created by lidor on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <Book.h>
#include <Frame.h>
#include <mutex>


using std::string;
using std::unordered_map;
using std::vector;
using std::mutex;

class User {
private:
    int subId;
    int receiptId;
    string name;
    bool loggedIn;
    unordered_map<string,string> genreSubId;
    unordered_map<string, vector<Book>> library;
    unordered_map<string,Frame&> receiptFrame;
    unordered_map<string,vector<string>> wishlist;
    mutex libraryLock;
    mutex wishlistLock;
    mutex genreSubLock;
    mutex receiptFrameLock;

public:
    User();
    void setUserName(string);
    string& getUserName();
    void login();
    void clear();
    bool isLoggedIn();
    int incrementSub();
    int incrementReceipt();
    void addBook(Book &book);
    bool bookExist(string genre,string bookName);
    void removeBook(string genre,string bookName);
    void addToWishlist(string genre,string bookName);
    string getWantedBook(string genre,string bookName);
    Book* getBook(string genre,string bookName);
    void setGenreSubId(string genre,string id);
    unordered_map<string,string>& getGenreSubId();
    vector<Book>& getGenreLibrary(string genre);
    void connectReceiptToFrame(string receipt, Frame &frame);
    Frame& getFrameFromReceipt(string receipt);

    unordered_map<string,Frame&> &getReceiptFrame();
};


#endif //BOOST_ECHO_CLIENT_USER_H