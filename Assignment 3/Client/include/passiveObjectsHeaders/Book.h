//
// Created by nadav0@wincs.cs.bgu.ac.il on 11/01/2020.
//

#include <string>

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

using namespace std;

class Book {
public:
    Book(string bookName, string previousOwner);

    string getPreviousOwner();

    void setPreviousOwner(string prevOwner);

    string getName();

    void acquireBook();

    void releaseBook();

    bool isAcquired();

private:
    string bookName;
    string previousOwner;
    bool isBelongToClient;
};

#endif //BOOST_ECHO_CLIENT_BOOK_H
