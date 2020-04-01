//
// Created by nadav0@wincs.cs.bgu.ac.il on 11/01/2020.
//
#include <passiveObjectsHeaders/Book.h>

Book::Book(string bookName, string previousOwner) : bookName(bookName), previousOwner(previousOwner),
                                                    isBelongToClient(true) {
}

string Book::getPreviousOwner() {
    return previousOwner;
}

void Book::setPreviousOwner(string prevOwner) {
    previousOwner = prevOwner;
}

string Book::getName() {
    return bookName;
}

void Book::acquireBook() {
    isBelongToClient = true;
}

void Book::releaseBook() {
    isBelongToClient = false;
}

bool Book::isAcquired() {
    return isBelongToClient;
}

