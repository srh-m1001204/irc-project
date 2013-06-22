#ifndef IRCBOT_H
#define IRCBOT_H

#include <vector>

#include "IRCLibrary.h"
#include "IRCMessageParser.h"
#include "LogDatabase.h"

class IRCBot {
    public:
    // constructor & destructor
        IRCBot(string nickname);
        ~IRCBot();

    // methods
        bool    ConnectToServer(string host, int port, string channel);
        bool    Start();

    private:
    // variables
        string  nickname;
        bool    logging;

    // methods
        bool    Loop();

        bool    BotAction(IRCMessageObject &messageObject);
        void    SendMessage(string message, string channel);
        void    SendPrivateMessage(string message, string receiver);

        void    CheckPing(string &buffer);
        void    CheckMessageAndSendResponse(IRCMessageObject &messageObject, string response);
        void    CheckBotChat(IRCMessageObject &messageObject);
        bool    CheckBotCommands(IRCMessageObject &messageObject);

        // bot functions
        void    ChangeNick(string nickname);
        void    ChangeTopic(string topic, string channel);
        void    JoinChannel(string channel);
        void    LeaveChannel(string channel);
        void    SetLogging(bool logging);
        void    LogMessage(IRCMessageObject messageObject);
        string  ShowLog();
        string  ShowLastSeen(string nickname);
};

#endif
