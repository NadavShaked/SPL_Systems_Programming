//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//
//#include <Client.h>
//#include <stdlib.h>
#include <ConnectionHandler.h>
#include <threadsHeaders/SocketReaderThread.h>
#include <threadsHeaders/SocketWriterThread.h>
#include <thread>
#include "string"

using namespace std;

#include "vector"

//login 132.72.45.156:5556 dan pass
void start(ConnectionHandler &connectionHandler, string userName, string passcode, string host);

vector<string> splitToWordsToVector(string str, char splitter);

string createFrameString(string stompCommand, vector<pair<string, string>> headers, string frameBody);

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main(int argc, char *argv[]) {

    string command;
    getline(cin, command);
    while (command != "bye") {
        vector<string> commandWords = splitToWordsToVector(command, ' ');
        if (commandWords.size() == 4 && commandWords.at(0) == "login") {
            vector<string> hostAndPort = splitToWordsToVector(commandWords.at(1), ':');
            string host = hostAndPort.at(0);
            short port = stoi(hostAndPort.at(1));
            string userName = commandWords.at(2);
            string passcode = commandWords.at(3);
            ConnectionHandler connectionHandler(host, port);
            if (!connectionHandler.connect()) {
                cerr << "Could not connect to server" << endl;
            } else {
                vector<pair<string, string>> headers;
                pair<string, string> acceptVersionHeader("accept-version", "1.2");
                pair<string, string> hostHeader("host", host);
                pair<string, string> loginHeader("login", userName);
                pair<string, string> passcodeHeader("passcode", passcode);
                headers.push_back(acceptVersionHeader);
                headers.push_back(hostHeader);
                headers.push_back(loginHeader);
                headers.push_back(passcodeHeader);

                string frameString = createFrameString("CONNECT", headers, "");
                connectionHandler.sendLine(frameString);
                string answer = "";
                connectionHandler.getLine(answer);
                if (answer.find("CONNECTED") != string::npos) {
                    cout << "Login successful" << endl;
                    start(connectionHandler, userName, passcode, host);
                } else if (answer.find("ERROR") != string::npos) {
                    if (answer.find("User already connected") != string::npos)
                        cout << "User already connected" << endl;
                    else if (answer.find("Wrong password") != string::npos)
                        cout << "Wrong password" << endl;
                }
            }
        }
        getline(cin, command);
    }
}

void start(ConnectionHandler &connectionHandler, string userName, string passcode, string host) {
    bool debugMode = false;

    Database *database = new Database(userName);
    StompProtocol stompProtocol(*database);
    SocketReaderThread socketReaderThread(connectionHandler, *database, stompProtocol);
    SocketWriterThread socketWriterThread(connectionHandler, *database, stompProtocol, host, userName, passcode);

    thread socketReaderThreadRunner(&SocketReaderThread::run, &socketReaderThread);
    thread socketWriterThreadRunner(&SocketWriterThread::run, &socketWriterThread);

    string command;
    getline(cin, command);
    while (command != "logout") {
        if (debugMode && command == "booklist") {
            database->printBookList();
        }
        database->addToMsgQueue(command);
        getline(cin, command);
    }


    map<int, string> topicsMap = database->getTopicsVector();
    for (auto &topic : topicsMap) {
        string genre = topic.second;
        int genreId = database->unsubscribeGenre(genre);

        if (genreId > 0) {
            int receiptId = database->getNewReceiptId("Exited club " + genre);
            vector<pair<string, string>> headers;
            pair<string, string> idHeader("id", to_string(genreId));
            pair<string, string> destinationHeader("destination", genre);
            pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
            headers.push_back(idHeader);
            headers.push_back(destinationHeader);
            headers.push_back(receiptIdHeader);

            string frameString = createFrameString("UNSUBSCRIBE", headers, "");
            connectionHandler.sendLine(frameString);
        }

    }


    int receiptId = database->getNewReceiptId("DISCONNECT");
    vector<pair<string, string>> headers;
    pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
    headers.push_back(receiptIdHeader);

    string frameString = createFrameString("DISCONNECT", headers, "");
    connectionHandler.sendLine(frameString);
    string answer = "";

    socketReaderThreadRunner.join();
    socketWriterThreadRunner.join();
    connectionHandler.close();
    delete (database);

    cout << "Disconnected" << endl;
}

vector<string> splitToWordsToVector(string str, char splitter) {
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

string createFrameString(string stompCommand, vector<pair<string, string>> headers, string frameBody) {
    string ret = stompCommand;
    for (auto &header : headers)
        ret += "\n" + header.first + ":" + header.second;
    ret += "\n\n" + frameBody;

    return ret;
}