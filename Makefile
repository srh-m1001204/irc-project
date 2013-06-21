all:
	g++ -c IRCMessageParser.cpp
	g++ -c IRCLibrary.cpp;
	g++ -c IRCBot.cpp;
	g++ main.cpp -o main IRCMessageParser.o IRCLibrary.o IRCBot.o;
