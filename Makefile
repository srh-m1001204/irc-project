SQLDIR	= /usr/bin/sqlite3
CCDIR	= /usr/bin/g++

CC	= g++
CFLAGS	= -g -Wall -ansi
LDFLAGS	= -lsqlite3
INCLUDE	= .
OBJ	= main.o LogDatabase.o IRCMessageParser.o IRCLibrary.o IRCBot.o

# executable
IRCBot:	$(OBJ)
	$(CC) $(CFLAGS) -o IRCBot $(OBJ) $(LDFLAGS)
	
%.o:	%.cpp
	$(CC) $(CFLAGS) -c $<
	
clean:	
	rm $(OBJ) IRCBot
	
all:	IRCBot

install: checkcc checksql

# check if g++ is installed
checkcc:
	@if [ ! -f $(CCDIR) ]; \
	then \
		sudo apt-get install g++;\
	else \
		echo "g++ is installed.";\
	fi

# check if sqlite3 is installed
checksql:	
	@if [ ! -f $(SQLDIR) ]; \
	then \
		mkdir tmpfolder; \
		cd tmpfolder; \
		wget http://www.sqlite.org/2013/sqlite-autoconf-3071700.tar.gz; \
		sudo tar xfvz sqlite-autoconf-3071700.tar.gz; \
		./sqlite-autoconf-3071700/configure; \
		sudo make install; \
		cd..;\
		sudo rm -r tmpfolder;\
		echo "installed sqlite3 in $(SQLDIR)"; \
	else \
		echo "sqlite3 is installed.";\
	fi