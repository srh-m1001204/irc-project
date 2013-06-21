#ifndef IRCLIBRARY_H
#define IRCLIBRARY_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <vector>
#include <sstream>

using namespace std;

#define BUFFER_SIZE 2048

class IRCLibrary {
    public:
        static bool     Connect(string host, int port);
        static void     Disconnect();
        static void     Identify(string username, string nickname, string password);
        static void     Send(string message);
        static string   Receive();
        static void     Parse(string &message);

        static void     SetCurrentChannel(string channel);
        static string   GetCurrentChannel();
    private:
        static int      ircSock;
        static string   currentChannel;

};

#endif
