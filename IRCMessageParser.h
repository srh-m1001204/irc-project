#ifndef IRCMESSAGEPARSER_H
#define IRCMESSAGEPARSER_H

#include <iostream>
#include <vector>

using namespace std;

enum IRCMessageType {
    IRC_PRIVMSG,
    IRC_JOIN,
    IRC_PART,
    IRC_TOPIC,
    IRC_NOTICE
};

class IRCMessageObject {
public:
    IRCMessageObject() {};
    ~IRCMessageObject() {};
    IRCMessageType  type;
    string          sender;
    string          channel;
    string          receiver;
    vector<string>  message;
};
class IRCMessageParser {
public:
    static IRCMessageObject ParseMessage(string message, bool &error);
};

#endif
