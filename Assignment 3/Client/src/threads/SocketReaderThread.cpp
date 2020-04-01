//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <threadsHeaders/SocketReaderThread.h>


SocketReaderThread::SocketReaderThread(ConnectionHandler &connectionHandler, Database &database,
                                       StompProtocol &stompProtocol) : connectionHandler(connectionHandler),
                                                                       database(database),
                                                                       stompProtocol(stompProtocol) {}

void SocketReaderThread::run() {
    while (!stompProtocol.shouldTerminate()) {
        string msg = "";
        if (connectionHandler.getLine(msg)) {
            stompProtocol.inputProcess(msg);
        }
    }
}
