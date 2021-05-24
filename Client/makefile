CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -pthread

all: Client
	g++ -o bin/StompBookClubClient bin/StompClient.o bin/ConnectionHandler.o bin/ServerHandler.o bin/StompMessagingProtocol.o bin/LineHandler.o bin/User.o bin/Frame.o bin/Book.o $(LDFLAGS)

Client: bin/ConnectionHandler.o bin/ServerHandler.o bin/StompMessagingProtocol.o bin/LineHandler.o bin/User.o bin/Frame.o bin/Book.o bin/StompClient.o
	
bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ServerHandler.o: src/ServerHandler.cpp
	g++ $(CFLAGS) -o bin/ServerHandler.o src/ServerHandler.cpp

bin/StompMessagingProtocol.o: src/StompMessagingProtocol.cpp
	g++ $(CFLAGS) -o bin/StompMessagingProtocol.o src/StompMessagingProtocol.cpp

bin/LineHandler.o: src/LineHandler.cpp
	g++ $(CFLAGS) -o bin/LineHandler.o src/LineHandler.cpp

bin/User.o: src/User.cpp
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/Book.o: src/Book.cpp
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
