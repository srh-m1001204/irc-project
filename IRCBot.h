#ifndef IRCBOT_H
#define IRCBOT_H

#include "IRCLibrary.h"
#include "IRCMessageParser.h"
#include "LogDatabase.h"
#include "ConfigFile.h"

class IRCBot {
    public:
    // constructor & destructor
        IRCBot();
        IRCBot(string nickname);
        ~IRCBot();

    // methods
        bool    ConnectToServer(string host, int port, string channel);
        void    Configure(int argc, char **argv, string path);
        bool    Start();

    private:
    // variables
        string  nickname;
        bool    logging;

    // methods
        void    CreateDefaultConfig();
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
        void    LogMessage(IRCMessageObject &messageObject);
        string  GetLog();
        string  GetLastSeen(string nickname);
};

#endif
