//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <mutex>
#include "Book.h"
#include "vector"
#include "map"
#include "queue"

#ifndef BOOST_ECHO_CLIENT_DATABASE_H
#define BOOST_ECHO_CLIENT_DATABASE_H

class Database {
public:
    Database(string userName);

    int subscribeGenre(string genre);

    int unsubscribeGenre(string genre);

    int getNewReceiptId(string receiptAnswer);

    void addBookToInventoryMap(string genre, Book book);

    string removeBookFromMap(string genre, string bookName);

    string getUserName();

    string findPrevOwnerInBookVector(vector<Book>, string bookName);

    string getTopicByTopicId(int topicId);

    void acquireBook(string genre, string bookName);

    void addToMsgQueue(string msg);

    string getBookList(string genre);

    string getReceiptAnswerById(int receiptId);

    void addBookToWishListMap(string genre, string bookName);

    bool isBookExistInInventory(string genre, string bookName);

    bool isInWishListBooks(string genre, string bookName);

    void borrowBookFromUser(string genre, string bookName, string previousOwner);

    void removeBookFromWishList(string genre, string bookName);

    void addBookToInventoryFromUser(string genre, string bookName, string previousOwner);

    void lendBookToUser(string genre, string bookName);

    string dequeueMsgQueue();

    bool isMsgQueueEmpty();

    map<int, string> getTopicsVector();

    void printBookList();

    ~Database();

private:
    mutex msgQueueLock;
    queue<string> newMsgQueue;
    mutex topicMapLock;
    map<int, string> newTopicMap;
    mutex receiptMapLock;
    map<int, string> newReceiptMap;
    mutex inventoryMapLock;
    map<string, vector<Book>> newInventoryMap;
    mutex wishListBooksLock;
    vector<pair<string, string>> wishListBooksVector; //first = genre , second = bookName

    int topicIdCounter;
    int receiptIdCounter;
    string userName;

    int getKeyByValue(string val);
};

#endif //BOOST_ECHO_CLIENT_DATABASE_H
