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
    if (msgType == "PRIVMSG")       msgObj.type = IRC_PRIVMSG;
    else if (msgType == "JOIN")     msgObj.type = IRC_JOIN;
    else if (msgType == "PART")     msgObj.type = IRC_PART;
    else if (msgType == "TOPIC")    msgObj.type = IRC_TOPIC;
    else if (msgType == "NOTICE")   msgObj.type = IRC_NOTICE;
    else if (msgType == "QUIT")     msgObj.type = IRC_QUIT;
    else {
        error = true;
        return msgObj;
    }
    string msgPart;
    int partNo = 0;
    pos1 = pos2;
    while ((pos2 = message.find(" ", pos1+1)) != string::npos) {
        msgPart = message.substr(++pos1, pos2-pos1-1);
        if (!msgPart.empty()) {
            IRCMessageParser::CheckAndAddPart(partNo++, msgPart, msgObj);
        } pos1 = pos2;
    }
    msgPart = message.substr(++pos1, pos2-pos1-1);
    if (!msgPart.empty())
        IRCMessageParser::CheckAndAddPart(partNo, msgPart, msgObj);

    cout << endl << "CHANNEL: " << msgObj.channel << endl << "RECEIVER: " << msgObj.receiver << endl << endl;
    for (vector<string>::iterator i=msgObj.message.begin(); i!=msgObj.message.end(); i++) {
        cout << "MSG-PART " << ": " << *i << "|||" << endl;
    }
    cout << endl << endl;
    error = false;
    return msgObj;
}
void IRCMessageParser::CheckAndAddPart(int partNo, string msgPart, IRCMessageObject &msgObj) {
    if (partNo == 0) {
        if (msgPart.at(0) == '#')   msgObj.channel = msgPart.substr(1);
        else                        msgObj.receiver = msgPart;
    } else if (partNo == 1 && msgPart.at(0) == ':') {
        msgObj.message.push_back(msgPart.substr(1));
    } else {
        msgObj.message.push_back(msgPart);
    }
}

