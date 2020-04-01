//
// Created by nadav0@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <passiveObjectsHeaders/StompProtocol.h>
#include <iostream>

StompProtocol::StompProtocol(Database &database) : database(database), isTerminated(false) {}

string StompProtocol::outputProcess(string msg) {
    if (msg.size() != 0) {
        vector<string> wordsVector = splitToWordsToVector(msg, ' ');
        if (wordsVector.size() > 0) {
            string command = wordsVector.at(0);
            if (command == "login") {
                printError("user already logged-in");
            } else if (command == "join") {
                if (wordsVector.size() == 2) {
                    string genre = wordsVector.at(1);
                    int genreId = database.subscribeGenre(genre);
                    if (genreId > 0) {
                        int receiptId = database.getNewReceiptId("Joined club " + genre);
                        vector<pair<string, string>> headers;
                        pair<string, string> destinationHeader("destination", genre);
                        pair<string, string> idHeader("id", to_string(genreId));
                        pair<string, string> receiptIdHeader("receipt",
                                                             to_string(receiptId));
                        headers.push_back(destinationHeader);
                        headers.push_back(idHeader);
                        headers.push_back(receiptIdHeader);

                        return createFrameString("SUBSCRIBE", headers, "");
                    } else {
                        printError("user already subscribed to this genre");
                    }
                }
            } else if (command == "exit") {
                if (wordsVector.size() == 2) {
                    string genre = wordsVector.at(1);
                    int genreId = database.unsubscribeGenre(genre);
                    if (genreId > 0) {
                        int receiptId = database.getNewReceiptId("Exited club " + genre);
                        vector<pair<string, string>> headers;
                        pair<string, string> idHeader("id", to_string(genreId));
                        pair<string, string> destinationHeader("destination", genre);
                        pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
                        headers.push_back(idHeader);
                        headers.push_back(destinationHeader);
                        headers.push_back(receiptIdHeader);

                        return createFrameString("UNSUBSCRIBE", headers, "");
                    } else {
                        printError("user wasn't subscribed to this genre");
                    }
                }
            } else if (command == "add") {
                if (wordsVector.size() >= 3) {
                    string genre = wordsVector.at(1);
                    string bookName = getBookName(wordsVector, 2);
                    Book book(bookName, "");
                    database.addBookToInventoryMap(genre, book);

                    vector<pair<string, string>> headers;
                    pair<string, string> destinationHeader("destination", genre);
                    headers.push_back(destinationHeader);
                    string frameBody = database.getUserName() + " has added the book " + bookName;

                    return createFrameString("SEND", headers, frameBody);
                } else {
                    printError("illegal command");
                }
            } else if (command == "borrow") {
                if (wordsVector.size() >= 3) {
                    string genre = wordsVector.at(1);
                    string bookName = getBookName(wordsVector, 2);
                    database.addBookToWishListMap(genre, bookName);

                    vector<pair<string, string>> headers;
                    pair<string, string> destinationHeader("destination", genre);
                    headers.push_back(destinationHeader);
                    string frameBody = database.getUserName() + " wish to borrow " + bookName;
                    return createFrameString("SEND", headers, frameBody);
                } else {
                    printError("illegal command");
                }
            } else if (command == "return") {
                if (wordsVector.size() >= 3) {
                    string genre = wordsVector.at(1);
                    string bookName = getBookName(wordsVector, 2);
                    string previousOwner = database.removeBookFromMap(genre, bookName);
                    if (previousOwner != "") {
                        vector<pair<string, string>> headers;
                        pair<string, string> destinationHeader("destination", genre);
                        headers.push_back(destinationHeader);
                        string frameBody = database.getUserName() + " Returning " + bookName + " to " +
                                           previousOwner;
                        return createFrameString("SEND", headers, frameBody);
                    } else {
                        printError("the book is yours kapara!, so we recycle the book");
                    }
                } else {
                    printError("illegal command");
                }
            } else if (command == "status") {
                if (wordsVector.size() == 2) {
                    string genre = wordsVector.at(1);
                    vector<pair<string, string>> headers;
                    pair<string, string> destinationHeader("destination", genre);
                    headers.push_back(destinationHeader);
                    string frameBody = "Book status";
                    return createFrameString("SEND", headers, frameBody);
                }
            } else if (command == "logout") {
                if (wordsVector.size() == 1) {
                    int receiptId = database.getNewReceiptId("DISCONNECT");
                    vector<pair<string, string>> headers;
                    pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
                    headers.push_back(receiptIdHeader);
                    return createFrameString("DISCONNECT", headers, "");
                } else {
                    printError("illegal command");
                }
            } else if (command == "publish") {
                string genre = wordsVector.at(1);
                vector<pair<string, string>> headers;
                pair<string, string> destinationHeader("destination", genre);
                headers.push_back(destinationHeader);
                string frameBody = database.getUserName() + ":" + database.getBookList(genre);
                return createFrameString("SEND", headers, frameBody);
            } else if (command == "haveBook") {
                string genre = wordsVector.at(1);
                string bookName = getBookName(wordsVector, 2);

                vector<pair<string, string>> headers;
                pair<string, string> destinationHeader("destination", genre);
                headers.push_back(destinationHeader);
                string frameBody = database.getUserName() + " has " + bookName;

                return createFrameString("SEND", headers, frameBody);
            } else if (command == "takingBook") {
                string genre = wordsVector.at(1);
                string previousOwner = wordsVector.at(2);
                string bookName = getBookName(wordsVector, 3);

                vector<pair<string, string>> headers;
                pair<string, string> destinationHeader("destination", genre);
                headers.push_back(destinationHeader);
                string frameBody = "Taking " + bookName + " from " + previousOwner;

                return createFrameString("SEND", headers, frameBody);
            } else {
                printError("illegal command");
            }
        }
    }
    return "";
}

vector<string> StompProtocol::splitToWordsToVector(string str, char splitter) {
    vector<string> wordsVector;
    string word = "";
    for (auto x : str) {
        if (x == splitter) {
            wordsVector.push_back(word);
            word = "";
        } else {
            word = word + x;
        }
    }
    wordsVector.push_back(word);
    return wordsVector;
}

void StompProtocol::printError(string error) {
    cout << error << endl;
}

string StompProtocol::createFrameString(string stompCommand, vector<pair<string, string>> headers, string frameBody) {
    string ret = stompCommand;
    for (auto &header : headers)
        ret += "\n" + header.first + ":" + header.second;
    ret += "\n\n" + frameBody;

    return ret;
}

string StompProtocol::getBookName(vector<string> commandLine, int startIndex) {
    string bookName = "";
    for (int i = startIndex; (unsigned) i < commandLine.size(); i++) {
        bookName += " " + commandLine.at(i);
    }
    return bookName.substr(1);
}

void StompProtocol::inputProcess(string msg) {
    vector<string> lines = splitToWordsToVector(msg, '\n');
    if (lines.size() == 0) {
        return;
    }
    string stompCommand = lines.at(0);
    vector<pair<string, string>> headers;
    string frameBody;
    int i = 1;


    while ((unsigned)i < lines.size() && lines.at(i).length() != 0) { //headers maker
        vector<string> headerNameAndValue = splitToWordsToVector(lines.at(i), ':');
        pair<string, string> header(headerNameAndValue.at(0), headerNameAndValue.at(1));
        headers.push_back(header);
        i++;
    }
    i++;

    frameBody = "";
    if ((unsigned)i < lines.size()) {
        frameBody = lines.at(i);
    }

    if (stompCommand == "CONNECTED") {
        cout << "Login successful." << endl;
    } else if (stompCommand == "RECEIPT") {
        string receiptId;
        for (auto &header : headers) {
            if (header.first == "receipt-id") {
                receiptId = header.second;
            }
        }
        string receiptAnswer = database.getReceiptAnswerById(stoi(receiptId));
        if (receiptAnswer == "DISCONNECT") {
            terminate();
        } else {
            cout << receiptAnswer << endl;
        }
    } else if (stompCommand == "MESSAGE") {
        string subscriptionId;
        for (auto &header : headers) {
            if (header.first == "subscription") {
                subscriptionId = header.second;
            }
        }
        int topicId = stoi(subscriptionId);
        string genre = database.getTopicByTopicId(topicId);
        cout << genre + ":" + frameBody << endl;


        if (frameBody.find("wish to borrow") != string::npos) {
            vector<string> line = splitToWordsToVector(frameBody, ' ');
            string bookName = getBookName(line, 4);
            if (database.isBookExistInInventory(genre, bookName)) {
                database.addToMsgQueue("haveBook " + genre + " " + bookName);
            }
        } else if (frameBody.find("has") != string::npos) {
            vector<string> line = splitToWordsToVector(frameBody, ' ');
            if (line.at(1) == "has") {
                string bookName = getBookName(line, 2);
                if (database.isInWishListBooks(genre, bookName)) {
                    string previousOwner = line.at(0);
                    database.borrowBookFromUser(genre, bookName, previousOwner);
                    database.addToMsgQueue("takingBook " + genre + " " + previousOwner + " " + bookName);
                }
            }
        } else if (frameBody.find("Returning") != string::npos) {
            vector<string> line = splitToWordsToVector(frameBody, ' ');
            if (database.getUserName().compare(line.at(line.size() - 1)) == 0) {
                string bookName = "";
                for (int j = 2; (unsigned) j < line.size() - 2; j++) {
                    bookName += " " + line.at(j);
                }
                bookName = bookName.substr(1);
                database.acquireBook(genre, bookName);
            }
        } else if (frameBody.find("Book status") != string::npos || frameBody.find("book status") != string::npos) {
            string msg = "publish " + genre;
            database.addToMsgQueue(msg);
        } else if (frameBody.find("Taking") != string::npos) {
            vector<string> line = splitToWordsToVector(frameBody, ' ');
            if (line.at(0) == "Taking" && database.getUserName() == line.at(line.size() - 1)) {
                string bookName = "";
                for (int j = 1; (unsigned) j < line.size() - 2; j++) {
                    bookName += " " + line.at(j);
                }
                bookName = bookName.substr(1);
                database.lendBookToUser(genre, bookName);
            }
        }
    } else if (stompCommand == "ERROR") {
        cout << frameBody << endl;
    }
}

void StompProtocol::terminate() {
    isTerminated = true;
}

bool StompProtocol::shouldTerminate() {
    return isTerminated;
}

