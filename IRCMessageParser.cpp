#include "IRCMessageParser.h"

// class IRCMessageObject
IRCMessageObject::IRCMessageObject() {}
IRCMessageObject::~IRCMessageObject() {}
// search for single string
int IRCMessageObject::Find(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    string temp;
    int index = 0;
    for (vector<string>::iterator it=message.begin(); it!=message.end(); it++) {
        temp = *it;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (temp.find(str) != string::npos)
            return index;
        index++;
    } return -1;
}
// search for multiple strings stored in an array
int IRCMessageObject::Find(string str[], size_t length) {
    string temp1, temp2;
    int index = 0;
    for (vector<string>::iterator it1=message.begin(); it1!=message.end(); it1++) {
        temp1 = *it1;
        transform(temp1.begin(), temp1.end(), temp1.begin(), ::tolower);
        for (size_t i=0; i<length; i++) {
            temp2 = str[i];
            transform(temp2.begin(), temp2.end(), temp2.begin(), ::tolower);
            if (temp1.find(temp2) != string::npos)
                return index;
        } index++;
    } return -1;
}
// search for multiple strings stored in a vector
int IRCMessageObject::Find(vector<string> strVec) {
    string temp1, temp2;
    int index = 0;
    for (vector<string>::iterator it1=message.begin(); it1!=message.end(); it1++) {
        temp1 = *it1;
        transform(temp1.begin(), temp1.end(), temp1.begin(), ::tolower);
        for (vector<string>::iterator it2=strVec.begin(); it2!=strVec.end(); it2++) {
            temp2 = *it2;
            transform(temp2.begin(), temp2.end(), temp2.begin(), ::tolower);
            if (temp1.find(temp2) != string::npos)
                return index;
        } index++;
    } return -1;
}


// class IRCMessageParse
IRCMessageObject IRCMessageParser::ParseMessage(string message, bool &error) {
    IRCMessageObject msgObj;
    size_t pos1, pos2;
    pos1 = message.find(":");
    pos2 = message.find("!");

    if (pos1 == string::npos || pos2 == string::npos) {
        error = true;
        return msgObj;
    }

    // parse sender of message
    pos1++;
    msgObj.sender = message.substr(pos1, pos2-pos1-1);

    pos1 = message.find(" ");
    pos2 = message.find(" ", pos1+1);

    // parse type of message
    pos1++;
    string msgType = message.substr(pos1, pos2-pos1-1);
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

    // parse all message parts
    string msgPart;
    int partNo = 0;
    pos1 = pos2;
    while ((pos2 = message.find(" ", pos1+1)) != string::npos) {
        pos1++;
        msgPart = message.substr(pos1, pos2-pos1-1);
        if (!msgPart.empty()) {
            IRCMessageParser::CheckAndAddPart(partNo++, msgPart, msgObj);
        } pos1 = pos2;
    }
    // parse last message part
    pos1++;
    msgPart = message.substr(pos1, pos2-pos1-1);
    if (!msgPart.empty())
        IRCMessageParser::CheckAndAddPart(partNo, msgPart, msgObj);

    error = false;
    return msgObj;
}
void IRCMessageParser::CheckAndAddPart(int partNo, string msgPart, IRCMessageObject &msgObj) {
    if (partNo == 0) {
        if (msgPart.at(0) == '#')   msgObj.channel = msgPart.substr(1);
        else                        msgObj.receiver = msgPart;
        return;
    }
    if (partNo == 1 && msgPart.at(0) == ':')
        msgPart = msgPart.substr(1);

    if (msgPart.find(":") != string::npos) {
        size_t pos1, pos2;
        string tmpPart;
        pos1 = 0;
        while ((pos2 = msgPart.find(":", pos1+1)) != string::npos) {
            if (pos2 != 0) {
                tmpPart = msgPart.substr(pos1, pos2-pos1+1);
                msgObj.message.push_back(tmpPart);
                if (pos2 == msgPart.length()-1)
                    break;
            } pos1 = pos2 + 1;
        } if (pos2 == string::npos) {
            tmpPart = msgPart.substr(pos1, msgPart.length()-pos1);
            msgObj.message.push_back(tmpPart);
        }
    } else {
        msgObj.message.push_back(msgPart);
    }
}
