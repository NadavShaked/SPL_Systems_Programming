//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <ConnectionHandler.h>
#include <passiveObjectsHeaders/Database.h>
#include <passiveObjectsHeaders/StompProtocol.h>

#ifndef BOOST_ECHO_CLIENT_SOCKETREADERTHREAD_H
#define BOOST_ECHO_CLIENT_SOCKETREADERTHREAD_H

#endif //BOOST_ECHO_CLIENT_SOCKETREADERTHREAD_H
using namespace std;

class SocketReaderThread {
public:
    SocketReaderThread(ConnectionHandler &connectionHandler, Database &database, StompProtocol &stompProtocol);

    void run();

private:
    ConnectionHandler &connectionHandler;
    Database &database;
    StompProtocol &stompProtocol;
};