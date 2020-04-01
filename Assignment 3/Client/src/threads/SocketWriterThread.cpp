//
// Created by nadav0@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <threadsHeaders/SocketWriterThread.h>
#include <passiveObjectsHeaders/Database.h>


void SocketWriterThread::run() {
    while (!stompProtocol.shouldTerminate()) {
        if (!database.isMsgQueueEmpty()) {
            string msg = database.dequeueMsgQueue();
            string framedMsg = stompProtocol.outputProcess(msg);
            if (!framedMsg.empty()) {
                connectionHandler.sendLine(framedMsg);
            }
        }
    }
}

SocketWriterThread::SocketWriterThread(ConnectionHandler &connectionHandler, Database &database,
                                       StompProtocol &stompProtocol, string host, string userName, string passcode)
        : connectionHandler(connectionHandler), database(database), stompProtocol(stompProtocol) {}


