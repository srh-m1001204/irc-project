#include "IRCMessageParser.h"

IRCMessageObject IRCMessageParser::ParseMessage(string message, bool &error) {
    IRCMessageObject msgObj;
    size_t pos1, pos2;
    pos1 = message.find(":");
    pos2 = message.find("!");

    if (pos1 == string::npos || pos2 == string::npos) {
        error = true;
        return msgObj;
    }

    msgObj.sender = message.substr(++pos1, pos2-pos1-1);
    cout << "Sender: " << msgObj.sender << endl;

    pos1 = message.find(" ");
    pos2 = message.find(" ", pos1+1);

    string msgType = message.substr(++pos1, pos2-pos1-1);
    if (msgType == "PRIVMSG")   msgObj.type = IRC_PRIVMSG;
    if (msgType == "JOIN")      msgObj.type = IRC_JOIN;
    if (msgType == "PART")      msgObj.type = IRC_PART;
    if (msgType == "TOPIC")     msgObj.type = IRC_TOPIC;
    if (msgType == "NOTICE")    msgObj.type = IRC_NOTICE;

    error = false;
    return msgObj;
}
