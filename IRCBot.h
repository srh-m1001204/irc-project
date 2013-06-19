#ifndef IRCBOT_H
#define IRCBOT_H

#include "IRCLibrary.h"

class IRCBot {
    public:
        IRCBot(string nickname);
        ~IRCBot();

        bool    ConnectToServer(string host, int port);
        bool    Start();

    private:
        string  nickname;

        bool    Loop();

        void    CheckPing(string &buffer);
};

#endif
