//
// Created by lidor on 16/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include <string>

using std::string;

class Book {
private:
    string genre;
    string owner;
    string name;
    bool available;

public:
    Book(string genre, string owner, string name);
    const string &getGenre() const;
    void setGenre(const string &genre);
    const string &getOwner() const;
    void setOwner(const string &owner);
    const string &getName() const;
    void setName(string name);
    bool isAvailable() const;
    void setAvailable(bool available);
};


#endif //BOOST_ECHO_CLIENT_BOOK_H
