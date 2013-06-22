#include "IRCBot.h"

IRCBot::IRCBot(string nickname) {
    this->nickname = nickname;
}
IRCBot::~IRCBot() {

}
bool IRCBot::Start() {
    return Loop();
}

bool IRCBot::ConnectToServer(string host, int port, string channel) {
    if (!IRCLibrary::Connect(host, port))
        return false;
    IRCLibrary::Login(nickname, channel);
    SetCurrentChannel(channel);
    return true;
}

bool IRCBot::Loop() {
    string              buffer;
    bool                receiveError,
                        parseError;
    IRCMessageObject    messageObject;

    while (true) {
        // get message from server
        buffer = IRCLibrary::Receive(receiveError);
        if (receiveError)
            return false;
        cout << buffer << endl;
        CheckPing(buffer);

        // parse buffer and create message object
        messageObject = IRCMessageParser::ParseMessage(buffer, parseError);

        // continue with loop if message is useless
        if (parseError)
            continue;

        // log message if logging is enabled
        if (logging)
            LogMessage(messageObject);

        // interpret message and perform bot function
        if (!BotAction(messageObject))
            return false;
    } return true;
}

bool IRCBot::BotAction(IRCMessageObject messageObject) {
    if (messageObject.type != IRC_PRIVMSG)
        return true;

    int pos;
    if (!messageObject.channel.empty()) {
        string str[3] = { "hallo", "hello", "hi" };
        if ((pos = messageObject.Find(str, 3)) != -1)
            SendMessage("Hallo " + messageObject.sender + "!");
    }




    return true;
}

void IRCBot::SendMessage(string message) {
    IRCLibrary::Send("PRIVMSG #" + currentChannel + " :" + message);
}
void IRCBot::SendPrivateMessage(string message, string receiver) {
    IRCLibrary::Send("PRIVMSG " + receiver + " :" + message);
}
void IRCBot::ChangeNick(string nickname) {
    this->nickname = nickname;
    IRCLibrary::Send("NICK " + nickname);
    SendMessage("Changed nickname into " + nickname);
}
void IRCBot::ChangeTopic(string topic, string channel) {
    IRCLibrary::Send("TOPIC " + channel + " " + topic);
    SendMessage("Tried to change topic into " + topic + " on channel " + channel);
}
void IRCBot::JoinChannel(string channel) {
    IRCLibrary::Send("JOIN #" + channel);
    SendMessage("Joined channel " + channel);
    SetCurrentChannel(channel);
}
void IRCBot::LeaveChannel(string channel) {
    SendMessage("Leaving channel " + channel + "... bye!");
    IRCLibrary::Send("PART #" + channel);
}
void IRCBot::LogMessage(IRCMessageObject messageObject) {
}


void IRCBot::CheckPing(string &buffer) {
    size_t pos = buffer.find("PING");
    if (pos != string::npos) {
        string pong = "PONG" + buffer.substr(pos + 4);
        cout << pong << endl;
        IRCLibrary::Send(pong);
    }
}
void IRCBot::CheckGreeting(IRCMessageObject &messageObject) {

}
void IRCBot::CheckBotCommands(IRCMessageObject &messageObject) {

}
void IRCBot::SetCurrentChannel(string channel) {
    currentChannel = channel;
}
string IRCBot::GetCurrentChannel() {
    return currentChannel;
}
