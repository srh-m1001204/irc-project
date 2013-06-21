#ifndef IRCBOT_H
#define IRCBOT_H

#include "IRCLibrary.h"

class IRCBot {
    public:
        IRCBot(string nickname);
        ~IRCBot();

        bool    ConnectToServer(string host, int port, string channel);
        bool    Start();

    private:
        string  nickname;

        bool    Loop();

        bool    BotAction(string message);
        void    ChangeNick(string nickname);
        void    ChangeTopic(string topic, string channel);
        void    JoinChannel(string channel);
        void    LeaveChannel(string channel);

        void    CheckPing(string &buffer);
};

#endif
