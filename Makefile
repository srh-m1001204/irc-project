all:
	g++ -c IRCLibrary.cpp;
	g++ -c IRCBot.cpp;
	g++ main.cpp -o main IRCLibrary.o IRCBot.o;
