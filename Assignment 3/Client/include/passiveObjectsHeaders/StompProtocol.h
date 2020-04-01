//
// Created by nadav0@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include "Database.h"

#ifndef BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPPROTOCOL_H


class StompProtocol {
public:
    StompProtocol(Database &database);

    string outputProcess(string msg);

    void inputProcess(string msg);

    string getBookName(vector<string> commandLine, int startIndex);

    string createFrameString(string stompCommand, vector<pair<string, string>> headers, string frameBody);

    static vector<string> splitToWordsToVector(string str, char splitter);

    void terminate();

    bool shouldTerminate();

private:
    Database &database;
    bool isTerminated;

    void printError(string errorMsg);
};

#endif //BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
