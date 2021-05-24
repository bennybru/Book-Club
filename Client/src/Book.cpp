//
// Created by lidor on 16/01/2020.
//

#include <Book.h>

Book::Book(string genre, string owner, string name) : genre(genre), owner(owner), name(name), available(true) {}

const string &Book::getGenre() const {
    return genre;
}

void Book::setGenre(const string &genre) {
    Book::genre = genre;
}

const string &Book::getOwner() const {
    return owner;
}

void Book::setOwner(const string &owner) {
    Book::owner = owner;
}

const string &Book::getName() const {
    return name;
}

void Book::setName(string name) {
    Book::name = name;
}

bool Book::isAvailable() const {
    return available;
}

void Book::setAvailable(bool available) {
    Book::available = available;
}
