#include "IRCBot.h"

IRCBot::IRCBot() {

}
IRCBot::~IRCBot() {

}
bool IRCBot::Start() {
    return Loop();
}

bool IRCBot::ConnectToServer(string host, int port) {
    if (!IRCLibrary::Connect(host, port))
        return false;
    return true;
}

bool IRCBot::Loop() {
    string buffer;
    while (true) {
        buffer = IRCLibrary::Receive();
        cout << buffer << endl;
        CheckPing(buffer);
        // BotFunctions
    }

    return true;
}
void IRCBot::CheckPing(string &buffer) {
    size_t pos = buffer.find("PING");
    if (pos != string::npos) {
        string pong = "PONG" + buffer.substr(pos + 4);
        cout << pong << endl;
        Send(pong);
    }
}
