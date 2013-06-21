#ifndef IRCBOT_H
#define IRCBOT_H

#include "IRCLibrary.h"

class IRCBot {
    public:
        IRCBot(string nickname);
        ~IRCBot();

        bool    ConnectToServer(string host, int port, string channel);
        bool    Start();

        void    SetCurrentChannel(string channel);
        string  GetCurrentChannel();
    private:
        string  nickname;
        string  currentChannel;

        bool    Loop();

        bool    BotAction(string message);
        void    SendMessage(string message);

        void    ChangeNick(string nickname);
        void    ChangeTopic(string topic);
        void    JoinChannel(string channel);
        void    LeaveChannel(string channel);

        void    CheckPing(string &buffer);
};

#endif
