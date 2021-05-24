//
// Created by lidor on 12/01/2020.
//

#include "User.h"

//constructors
User::User() :
        subId(0), receiptId(0), name(), loggedIn(false), genreSubId(), library(), receiptFrame(), wishlist() {}

//user related methods
void User::setUserName(string name) {
    this->name = name;
}

string& User::getUserName() {
    return name;
}

void User::login() {
    loggedIn = true;
}

bool User::isLoggedIn() {
    return loggedIn;
}

//book management methods
int User::incrementSub() {
    return subId++;
}

int User::incrementReceipt() {
    return receiptId++;
}

void User::addBook(Book &book) {
    vector<Book> tmp;
    libraryLock.lock();
    tmp = library[book.getGenre()];
    tmp.push_back(book);
    libraryLock.unlock();
}

bool User::bookExist(string genre,string bookName) {
    vector<Book> tmp;
    libraryLock.lock();
    tmp = library[genre];
    for (int i=0;i<tmp.size();i++) {
        if (tmp[i].getName() == bookName && tmp[i].isAvailable()) {
            libraryLock.unlock();
            return true;
        }
    }
    libraryLock.unlock();
    return false;
}

void User::removeBook(string genre,string bookName) {
    vector<Book> tmp;
    libraryLock.lock();
    tmp = library[genre];
    for (int i=0;i<tmp.size();i++) {
        if (tmp[i].getName() == bookName) {
            delete(&tmp[i]);
            tmp.erase(tmp.begin()+i);
            break;
        }
    }
    libraryLock.unlock();
}

void User::addToWishlist(string genre, string bookName) {
    vector<string> tmp;
    wishlistLock.lock();
    tmp = wishlist[genre];
    tmp.push_back(bookName);
    wishlistLock.unlock();
}

string User::getWantedBook(string genre, string bookName) {
    vector<string> tmp;
    wishlistLock.lock();
    tmp = wishlist[genre];
    for (int i=0;i<tmp.size();i++) {
        if (tmp[i] == bookName) {
            wishlistLock.unlock();
            return tmp[i];
        }
    }
    wishlistLock.unlock();
    return "";
}

Book* User::getBook(string genre, string bookName) {
    vector<Book> tmp;
    libraryLock.lock();
    tmp = library[genre];
    for (int i=0;i<tmp.size();i++) {
        if (tmp[i].getName() == bookName) {
            libraryLock.unlock();
            return &tmp[i];
        }
    }
    libraryLock.unlock();
    return nullptr;
}

void User::setGenreSubId(string genre, string id) {
    genreSubLock.lock();
    genreSubId[genre] = id;
    genreSubLock.unlock();
}


vector<Book> &User::getGenreLibrary(string genre) {
    libraryLock.lock();
    vector<Book>& tmp = library[genre];
    libraryLock.unlock();
    return tmp;
}

//receipts-tasks helpers
void User::connectReceiptToFrame(string receipt, Frame &frame) {
    auto pair = std::pair<string,Frame&>(receipt,frame);
    receiptFrameLock.lock();
    receiptFrame.insert(pair);
    receiptFrameLock.unlock();
}

Frame& User::getFrameFromReceipt(string receipt) {
    receiptFrameLock.lock();
    auto tmp = receiptFrame.find(receipt);
    receiptFrameLock.unlock();
    return tmp->second;
}

unordered_map<string, Frame&> &User::getReceiptFrame() {
    return receiptFrame;
}

unordered_map<string, string> &User::getGenreSubId() {
    return genreSubId;
}

void User::clear() {
    for (auto genre : library) {
        for (Book book : genre.second) {
            delete(&book);
        }
    }
    for (auto receipt : receiptFrame) {
        Frame frame = receipt.second;
        auto headers = frame.getHeaders();
        delete(&headers);
        delete(&frame);
    }
}
