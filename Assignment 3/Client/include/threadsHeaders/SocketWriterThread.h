//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <ConnectionHandler.h>
#include <passiveObjectsHeaders/Database.h>
#include <passiveObjectsHeaders/StompProtocol.h>
#include "mutex"

#ifndef BOOST_ECHO_CLIENT_SOCKETWRITERTHREAD_H
#define BOOST_ECHO_CLIENT_SOCKETWRITERTHREAD_H

#endif //BOOST_ECHO_CLIENT_SOCKETWRITERTHREAD_H
using namespace std;

class SocketWriterThread {
public:
    SocketWriterThread(ConnectionHandler &connectionHandler, Database &database, StompProtocol &stompProtocol,
                       string host, string userName, string passcode);

    void run();

private:
    ConnectionHandler &connectionHandler;
    Database &database;
    StompProtocol &stompProtocol;
};