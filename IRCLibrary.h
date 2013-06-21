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
        static void     Login(string nickname, string channel);
        static void     Send(string message);
        static string   Receive(bool &error);
        static void     Parse(string &message);

    private:
        static int      ircSock;
};

#endif
