#include "IRCBot.h"

IRCBot::IRCBot() {
    LogDatabase::InitDatabase("logDatabase.sqlite");
}
IRCBot::IRCBot(string nickname) {
    this->nickname = nickname;
    this->logging = false;
    LogDatabase::InitDatabase("logDatabase.sqlite");
}
IRCBot::~IRCBot() {
}
bool IRCBot::ConnectToServer(string host, int port, string channel) {
    if (!IRCLibrary::Connect(host, port))
        return false;
    IRCLibrary::Login(nickname, channel);
    return true;
}
void IRCBot::Configure(int argc, char **argv, string path) {

}
bool IRCBot::Start() {
    if (!ConnectToServer("irc.freenode.net", 6667, "CrazyChannel"))
        return false;
    return Loop();
}
void IRCBot::CreateDefaultConfig() {

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

bool IRCBot::BotAction(IRCMessageObject &messageObject) {
    if (messageObject.type != IRC_PRIVMSG)
        return true;

    CheckBotChat(messageObject);
    return CheckBotCommands(messageObject);
}
void IRCBot::SendMessage(string message, string channel) {
    IRCLibrary::Send("PRIVMSG #" + channel + " :" + message);
}
void IRCBot::SendPrivateMessage(string message, string receiver) {
    IRCLibrary::Send("PRIVMSG " + receiver + " :" + message);
}

void IRCBot::CheckPing(string &buffer) {
    size_t pos = buffer.find("PING");
    if (pos != string::npos) {
        string pong = "PONG" + buffer.substr(pos + 4);
        cout << pong << endl;
        IRCLibrary::Send(pong);
    }
}
void IRCBot::CheckMessageAndSendResponse(IRCMessageObject &messageObject, string response) {
    // checks if message is a private message
    if (messageObject.receiver == nickname)
        SendPrivateMessage(response, messageObject.sender);
    else
        SendMessage(response, messageObject.channel);
}
void IRCBot::CheckBotChat(IRCMessageObject &messageObject) {
    {   // checks if some sent a greeting message
        string str[3] = { "hallo", "hello", "hi" };
        if (messageObject.Find(str, 3) != -1)
            CheckMessageAndSendResponse(messageObject, "Hello " + messageObject.sender + "!");
    }
    {   // ...
        // more bot chat options
    }
}
bool IRCBot::CheckBotCommands(IRCMessageObject &messageObject) {
    //check if bot is addressed or if message is a private message
    if ((messageObject.message[0] != (nickname + ":")) && (messageObject.receiver != nickname))
        return true;    // ignore message if not

    // bot functions
    int pos;
    if ((pos = messageObject.Find("-logging")) != -1) {
        if ((int)messageObject.message.size()-1 < pos+1) {
            CheckMessageAndSendResponse(messageObject, "command needs 1 parameter...");
        } else if (messageObject.message[pos+1] == "on") {
            SetLogging(true);
            CheckMessageAndSendResponse(messageObject, "logging is enabled!");
        } else if (messageObject.message[pos+1] == "off") {
            SetLogging(false);
            CheckMessageAndSendResponse(messageObject, "logging is disabled!");
        } else {
            CheckMessageAndSendResponse(messageObject, "wrong parameter... type 'on'/'off' as parameter");
        }
    } else if ((pos = messageObject.Find("-nick")) != -1) {
        if ((int)messageObject.message.size()-1 < pos+1) {
            CheckMessageAndSendResponse(messageObject, "command needs 1 parameter...");
        } else {
            ChangeNick(messageObject.message[pos+1]);
            CheckMessageAndSendResponse(messageObject, "changed nickname into " + messageObject.message[pos+1] + "...");
        }
    } else if ((pos = messageObject.Find("-join")) != -1) {
        if ((int)messageObject.message.size()-1 < pos+1) {
            CheckMessageAndSendResponse(messageObject, "command needs 1 parameter...");
        } else {
            JoinChannel(messageObject.message[pos+1]);
            CheckMessageAndSendResponse(messageObject, "joined channel " + messageObject.message[pos+1] + "...");
        }
    } else if ((pos = messageObject.Find("-leave")) != -1) {
        if ((int)messageObject.message.size()-1 < pos+1) {
            CheckMessageAndSendResponse(messageObject, "command needs 1 parameter...");
        } else {
            CheckMessageAndSendResponse(messageObject, "leaving channel " + messageObject.message[pos+1] + "...");
            LeaveChannel(messageObject.message[pos+1]);
        }
    } else if ((pos = messageObject.Find("-topic")) != -1) {
        if ((int)messageObject.message.size()-1 < pos+2) {
            CheckMessageAndSendResponse(messageObject, "command needs 2 parameter...");
        } else {
            ChangeTopic(messageObject.message[pos+1], messageObject.message[pos+2]);
            CheckMessageAndSendResponse(messageObject, "tried to change topic on channel "
                    + messageObject.message[pos+1] + " into " + messageObject.message[pos+1] + "...");
        }
    } else if ((pos = messageObject.Find("-exit")) != -1) {
        CheckMessageAndSendResponse(messageObject, "disconnecting... bye bye!");
        return false;
    } else if ((pos = messageObject.Find("-get_log")) != -1) {
        CheckMessageAndSendResponse(messageObject, GetLog());
    } else if ((pos = messageObject.Find("-get_lastseen")) != -1) {
        if ((int)messageObject.message.size()-1 < pos+1) {
            CheckMessageAndSendResponse(messageObject, "command needs 1 parameter...");
        } else {
            CheckMessageAndSendResponse(messageObject, GetLastSeen(messageObject.message[pos+1]));
        }
    } else {
        CheckMessageAndSendResponse(messageObject, "Sorry " + messageObject.sender + " , I didn't understand...");
    }
    return true;
}

void IRCBot::ChangeNick(string nickname) {
    this->nickname = nickname;
    IRCLibrary::Send("NICK " + nickname);
}
void IRCBot::ChangeTopic(string topic, string channel) {
    IRCLibrary::Send("TOPIC " + channel + " " + topic);
}
void IRCBot::JoinChannel(string channel) {
    IRCLibrary::Send("JOIN #" + channel);
}
void IRCBot::LeaveChannel(string channel) {
    IRCLibrary::Send("PART #" + channel);
}
void IRCBot::SetLogging(bool logging) {
    this->logging = logging;
}
void IRCBot::LogMessage(IRCMessageObject &messageObject) {
    switch(messageObject.type) {
    case IRC_PRIVMSG:
        LogDatabase::InsertLog(messageObject.sender, messageObject.channel, messageObject.GetMessageString());
        break;
    default:
        break;
    };
}
string IRCBot::GetLog() {
    return LogDatabase::GetLog();
}
string IRCBot::GetLastSeen(string nickname) {
    return LogDatabase::LastSeen(nickname);
}
