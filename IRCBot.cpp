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
    Login(nickname, channel);
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
void IRCBot::CheckPing(string &buffer) {
    size_t pos = buffer.find("PING");
    if (pos != string::npos) {
        string pong = "PONG" + buffer.substr(pos + 4);
        cout << pong << endl;
        IRCLibrary::Send(pong);
    }
}
