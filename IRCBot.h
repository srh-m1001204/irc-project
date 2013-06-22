#ifndef IRCBOT_H
#define IRCBOT_H

#include <vector>

#include "IRCLibrary.h"
#include "IRCMessageParser.h"

class IRCBot {
    public:
    // constructor & destructor
        IRCBot(string nickname);
        ~IRCBot();

    // methods
        bool    ConnectToServer(string host, int port, string channel);
        bool    Start();

        // set methods
        void    SetCurrentChannel(string channel);
        void    SetLogging(bool logging);
        // get methods
        string  GetCurrentChannel();
        bool    GetLogging();

    private:
    // variables
        string  nickname;
        string  currentChannel;
        bool    logging;

    // methods
        bool    Loop();

        bool    BotAction(IRCMessageObject messageObject);
        void    SendMessage(string message);
        void    SendPrivateMessage(string message, string receiver);

        // bot functions
        void    ChangeNick(string nickname);
        void    ChangeTopic(string topic, string channel);
        void    JoinChannel(string channel);
        void    LeaveChannel(string channel);
        void    LogMessage(IRCMessageObject messageObject);

        void    CheckPing(string &buffer);
};

#endif
