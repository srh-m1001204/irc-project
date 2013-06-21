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
    string buffer;
    bool error;
    while (true) {
        buffer = IRCLibrary::Receive(error);
        if (error)
            return false;
        cout << buffer << endl;
        CheckPing(buffer);
        if (!BotAction(buffer))
            return false;
    } return true;
}

bool IRCBot::BotAction(string message) {


}

void IRCBot::SendMessage(string message) {
    IRCLibrary::Send("PRIVMSG #" + currentChannel + " :" + message);
}
void IRCBot::ChangeNick(string nickname) {
    this->nickname = nickname;
    IRCLibrary::Send("NICK " + nickname);
    SendMessage("Changed nickname into " + nickname);
}
void IRCBot::ChangeTopic(string topic, string channel) {
    IRCLibrary::Send("TOPIC " + currentChannel + " " + topic);
    SendMessage("Tried to change topic into " + topic);
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

void IRCBot::CheckPing(string &buffer) {
    size_t pos = buffer.find("PING");
    if (pos != string::npos) {
        string pong = "PONG" + buffer.substr(pos + 4);
        cout << pong << endl;
        IRCLibrary::Send(pong);
    }
}

void IRCBot::SetCurrentChannel(string channel) {
    currentChannel = channel;
}
string IRCBot::GetCurrentChannel() {
    return currentChannel;
}
