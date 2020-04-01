CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: StompBookClubClient
	g++ -pthread -o bin/StompBookClubClient bin/Book.o bin/Database.o bin/StompProtocol.o bin/SocketReaderThread.o bin/SocketWriterThread.o bin/ConnectionHandler.o bin/main.o $(LDFLAGS) 

StompBookClubClient: bin/Book.o bin/Database.o bin/StompProtocol.o bin/SocketReaderThread.o bin/SocketWriterThread.o bin/ConnectionHandler.o bin/main.o
	
bin/Book.o: src/passiveObjects/Book.cpp
	g++ -pthread $(CFLAGS) -o bin/Book.o src/passiveObjects/Book.cpp

bin/Database.o: src/passiveObjects/Database.cpp
	g++ -pthread $(CFLAGS) -o bin/Database.o src/passiveObjects/Database.cpp

bin/StompProtocol.o: src/passiveObjects/StompProtocol.cpp
	g++ -pthread $(CFLAGS) -o bin/StompProtocol.o src/passiveObjects/StompProtocol.cpp

bin/SocketReaderThread.o: src/threads/SocketReaderThread.cpp
	g++ -pthread $(CFLAGS) -o bin/SocketReaderThread.o src/threads/SocketReaderThread.cpp

bin/SocketWriterThread.o: src/threads/SocketWriterThread.cpp
	g++ -pthread $(CFLAGS) -o bin/SocketWriterThread.o src/threads/SocketWriterThread.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/main.o: src/main.cpp
	g++ -pthread $(CFLAGS) -o bin/main.o src/main.cpp

	
.PHONY: clean
clean:
	rm -f bin/*
