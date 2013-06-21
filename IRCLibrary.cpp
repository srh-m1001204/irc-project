#include "IRCLibrary.h"

int IRCLibrary::ircSock = 0;

bool IRCLibrary::Connect(string host, int port) {
    // create socket
    ircSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ircSock < 0) {
        perror("Error: Failed to create socket!");
        Disconnect();
        return false;
    }
    // get host info
    hostent *hostInfo = gethostbyname(host.c_str());
    if (!hostInfo) {
        perror("Error: Failed to get host information!");
        Disconnect();
        return false;
    }
    // set server info
    sockaddr_in serverInfo;
    memset((char*)&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    memcpy((char*)&serverInfo.sin_addr, hostInfo->h_addr, hostInfo->h_length);
    serverInfo.sin_port = htons(port);
    memset(&(serverInfo.sin_zero), 0, 8*sizeof(char));

    // connect socket
    if (connect(ircSock, (sockaddr*) &serverInfo, sizeof(serverInfo)) == -1) {
        perror("Error: Failed to connect socket!");
        Disconnect();
        return false;
    } return true;
}
void IRCLibrary::Disconnect() {
    close(ircSock);
}
void IRCLibrary::Login(string nickname, string channel) {
    Send("NICK " + nickname);
    Send("JOIN #" + channel);
}
void IRCLibrary::Send(string message) {
    message += "\r\n";
    send(ircSock, message.c_str(), message.length(), 0);
}
string IRCLibrary::Receive(bool &error) {
    error = false;
    char buffer[BUFFER_SIZE+1] = {0};
    if (recv(ircSock, buffer, BUFFER_SIZE, 0) < 0) {
        perror("Error: Failed to receive message!");
        error = true;
        return "";
    }
    string sBuffer = string(buffer);
    Parse(sBuffer);
    return sBuffer;
}
void IRCLibrary::Parse(string &message) {
    if (message.find("\r\n") == message.length()-2)
        message.erase(message.length()-2);
}
