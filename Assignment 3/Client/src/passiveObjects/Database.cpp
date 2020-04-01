//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//
#include <passiveObjectsHeaders/Database.h>
#include <iostream>

Database::Database(string userName) : msgQueueLock(), newMsgQueue(), topicMapLock(), newTopicMap(), receiptMapLock(), newReceiptMap(), inventoryMapLock(), newInventoryMap(), wishListBooksLock(), wishListBooksVector(), topicIdCounter(0), receiptIdCounter(0), userName(userName) {}

int Database::subscribeGenre(string genre) {
    topicMapLock.lock();
    if (getKeyByValue(genre) < 0) { // not subscribed to topic already
        topicIdCounter += 1;
        newTopicMap.emplace(topicIdCounter, genre);
        topicMapLock.unlock();
        return topicIdCounter;
    }
    topicMapLock.unlock();
    return -1;
}

int Database::unsubscribeGenre(string genre) {
    topicMapLock.lock();
    int genreId = getKeyByValue(genre);
    if (genreId > 0) {
        newTopicMap.erase(genreId);
        topicMapLock.unlock();
        return genreId;
    }
    topicMapLock.unlock();
    return -1;
}

int Database::getKeyByValue(string val) {
    int toReturn = -1;
    for (auto &topic : newTopicMap) {
        if (topic.second == val) {
            toReturn = topic.first;
        }
    }
    return toReturn;
}

int Database::getNewReceiptId(string receiptAnswer) {
    receiptMapLock.lock();
    receiptIdCounter += 1;
    newReceiptMap.emplace(receiptIdCounter, receiptAnswer);
    receiptMapLock.unlock();
    return receiptIdCounter;
}

void Database::addBookToInventoryMap(string genre, Book book) {
    inventoryMapLock.lock();
    if (newInventoryMap.find(genre) != newInventoryMap.end()) {
        newInventoryMap.at(genre).push_back(book);
    } else {
        vector<Book> booksVector;
        booksVector.push_back(book);
        newInventoryMap.emplace(genre, booksVector);
    }
    inventoryMapLock.unlock();
}

string Database::getUserName() {
    return userName;
}

string Database::removeBookFromMap(string genre, string bookName) {
    inventoryMapLock.lock();
    if (newInventoryMap.find(genre) != newInventoryMap.end()) {
        string prevOwner = findPrevOwnerInBookVector(newInventoryMap.at(genre), bookName);
        for (int i = 0; (unsigned) i <  newInventoryMap.at(genre).size(); i++) {
            if (newInventoryMap.at(genre).at(i).getName() == bookName)
                newInventoryMap.at(genre).erase(
                        newInventoryMap.at(genre).begin() + i);
        }
        inventoryMapLock.unlock();
        return prevOwner;
    } else {
        cout << "book doesn't exist" << endl;
    }
    inventoryMapLock.unlock();
    return "";
}

string Database::findPrevOwnerInBookVector(vector<Book> vector, string bookName) {
    for (auto &book : vector) {
        if (bookName == book.getName()) {
            return book.getPreviousOwner();
        }
    }
    return "";
}

string Database::getTopicByTopicId(int topicId) {
    topicMapLock.lock();
    string topic = newTopicMap.at(topicId);
    topicMapLock.unlock();
    return topic;
}

void Database::acquireBook(string genre, string bookName) {
    inventoryMapLock.lock();
    if (newInventoryMap.find(genre) != newInventoryMap.end()) {
        for (auto &book: newInventoryMap.at(genre)) {
            if (book.getName() == bookName) {
                book.acquireBook();
            }
        }
    }
    inventoryMapLock.unlock();
}

void Database::addToMsgQueue(string msg) {
    msgQueueLock.lock();
    newMsgQueue.emplace(msg);
    msgQueueLock.unlock();
}

string Database::getBookList(string genre) {
    inventoryMapLock.lock();
    string bookList = "";

    if (newInventoryMap.find(genre) != newInventoryMap.end()) {
        for (auto &book : newInventoryMap.at(genre)) {
            if (book.isAcquired())
                bookList += "," + book.getName();
        }
        if (bookList.size() != 0)
            bookList = bookList.substr(1);
    }
    inventoryMapLock.unlock();
    return bookList;
}

string Database::getReceiptAnswerById(int receiptId) {
    receiptMapLock.lock();
    string receiptAnswer = "no available receipt";
    if (newReceiptMap.find(receiptId) != newReceiptMap.end())
        receiptAnswer = newReceiptMap.at(receiptId);
    receiptMapLock.unlock();
    return receiptAnswer;
}

void Database::addBookToWishListMap(string genre, string bookName) {
    wishListBooksLock.lock();
    pair<string, string> book(genre, bookName);
    wishListBooksVector.push_back(book);
    wishListBooksLock.unlock();
}

bool Database::isBookExistInInventory(string genre, string bookName) {
    inventoryMapLock.lock();
    if (newInventoryMap.find(genre) != newInventoryMap.end() && newInventoryMap.at(genre).size() != 0) {
        for (auto &book : newInventoryMap.at(genre)) {
            if (book.isAcquired() && book.getName() == bookName) {
                inventoryMapLock.unlock();
                return true;
            }
        }
    }
    inventoryMapLock.unlock();
    return false;
}

bool Database::isInWishListBooks(string genre, string bookName) {
    wishListBooksLock.lock();
    bool ans = false;
    for (auto &book : wishListBooksVector) {
        if (book.first == genre && book.second == bookName) {
            ans = true;
        }
    }
    wishListBooksLock.unlock();
    return ans;
}

void Database::borrowBookFromUser(string genre, string bookName, string previousOwner) {
    removeBookFromWishList(genre, bookName);
    addBookToInventoryFromUser(genre, bookName, previousOwner);
}

void Database::removeBookFromWishList(string genre, string bookName) {
    wishListBooksLock.lock();
    for (int i = 0; (unsigned) i < wishListBooksVector.size(); i++) {
        if (wishListBooksVector.at(i).first == genre && wishListBooksVector.at(i).second == bookName) {
            wishListBooksVector.erase(wishListBooksVector.begin() + i);
            break;
        }
    }
    wishListBooksLock.unlock();
}

void Database::addBookToInventoryFromUser(string genre, string bookName, string previousOwner) {
    Book newBook(bookName, previousOwner);
    inventoryMapLock.lock();
    if (newInventoryMap.find(genre) == newInventoryMap.end()) {
        vector<Book> books;
        newInventoryMap.emplace(genre, books);
    }
    newInventoryMap.at(genre).push_back(newBook);

    inventoryMapLock.unlock();
}

void Database::lendBookToUser(string genre, string bookName) {
    inventoryMapLock.lock();
    if (newInventoryMap.find(genre) != newInventoryMap.end()) {
        for (auto &book : newInventoryMap.at(genre)) {
            if (book.getName() == bookName) {
                book.releaseBook();
                break;
            }
        }
    }
    inventoryMapLock.unlock();
}

string Database::dequeueMsgQueue() {
    msgQueueLock.lock();
    if (newMsgQueue.size() > 0) {
        string msg = newMsgQueue.front();
        newMsgQueue.pop();
        msgQueueLock.unlock();
        return msg;
    } else {
        msgQueueLock.unlock();
        return "";
    }
}

bool Database::isMsgQueueEmpty() {
    return (newMsgQueue.size() == 0);
}

void Database::printBookList() {
    cout << userName << endl;
    for (auto &genre_bookListOfGenre : newInventoryMap) {
        cout << "\n" + genre_bookListOfGenre.first << endl;
        for (auto &book: genre_bookListOfGenre.second) {
            string isAqu = "";
            if (book.isAcquired()) {
                isAqu = "true";
            } else {
                isAqu = "false";
            }
            cout << book.getName() + "-" + isAqu + ", " << endl;
        }
    }
}

map<int, string> Database::getTopicsVector() {
    topicMapLock.lock();
    map<int, string> retMap = newTopicMap;
    topicMapLock.unlock();
    return retMap;
}

Database::~Database() {
    newTopicMap.clear();
    newReceiptMap.clear();
    wishListBooksVector.clear();
    newInventoryMap.clear();
    while (!newMsgQueue.empty()) {
        newMsgQueue.pop();
    }
}
