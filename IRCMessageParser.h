#ifndef IRCMESSAGEPARSER_H
#define IRCMESSAGEPARSER_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum IRCMessageType {
    IRC_PRIVMSG,
    IRC_JOIN,
    IRC_PART,
    IRC_TOPIC,
    IRC_NOTICE,
    IRC_QUIT
};
class IRCMessageObject {
public:
    IRCMessageObject();
    ~IRCMessageObject();
    IRCMessageType  type;
    string          sender;
    string          channel;
    string          receiver;
    vector<string>  message;

    int             Find(string str);
    int             Find(vector<string> strVec);
};
class IRCMessageParser {
public:
    static IRCMessageObject ParseMessage(string message, bool &error);
    static void             CheckAndAddPart(int partNo, string msgPart, IRCMessageObject &msgObj);
};

#endif
