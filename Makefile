SQLDIR	= /usr/bin/sqlite3
CCDIR	= /usr/bin/g++

CC	= g++
CFLAGS	= -g -Wall -ansi
LDFLAGS	= -lsqlite3
INCLUDE	= .
OBJ	= main.o ConfigFile.o LogDatabase.o IRCMessageParser.o IRCLibrary.o IRCBot.o

# executable
IRCBot:	$(OBJ)
	$(CC) $(CFLAGS) -o IRCBot $(OBJ) $(LDFLAGS)
	
%.o:	%.cpp
	$(CC) $(CFLAGS) -c $<
	
clean:	
	rm $(OBJ) IRCBot
	
all:	checkcc checksql IRCBot

install: checkcc checksql

# check if g++ is installed
checkcc:
	@if [ ! -f $(CCDIR) ]; \
	then \
		sudo apt-get install g++;\
		echo "installed g++ in $(CCDIR)"; \
	else \
		echo "g++ is already installed.";\
	fi

# check if sqlite3 is installed
checksql:	
	@if [ ! -f $(SQLDIR) ]; \
	then \
		sudo apt-get install libsqlite3-dev sqlite3;\
		echo "installed sqlite3 in $(SQLDIR)"; \
	else \
		echo "sqlite3 is already installed.";\
	fi