#include "IRCBot.h"

IRCBot::IRCBot() {
    LogDatabase::InitDatabase("logDatabase.sqlite");
    logging = true;
    configFile = NULL;
}
IRCBot::IRCBot(string nickname) {
    this->nickname = nickname;
    this->logging = false;
    LogDatabase::InitDatabase("logDatabase.sqlite");
    configFile = NULL;
}
IRCBot::~IRCBot() {
    delete configFile;
}
bool IRCBot::ConnectToServer(string host, int port, string channel) {
    if (!IRCLibrary::Connect(host, port))
        return false;
    IRCLibrary::Login(nickname, channel);
    return true;
}
bool IRCBot::Configure(int argc, char **argv, string path) {
    configFile = new ConfigFile(path);
    // try to open config file.
    // if not possible, create new default config file
    if (!configFile->Load())
        CreateDefaultConfig();

    // check entered arguments and edit config file
    switch (argc) {
    case 1:         // case if no arguments were entered
        break;
    case 2:         // 1 argument:  (nickname)
        configFile->SetValueOf("nickname", string(argv[1]));
        break;
    case 4:         // 3 arguments: (host, port, channel)
        configFile->SetValueOf("host", string(argv[1]));
        configFile->SetValueOf("port", string(argv[2]));
        configFile->SetValueOf("channel", string(argv[3]));
        break;
    case 5:         // 4 arguments: (host, port, channel, nickname)
        configFile->SetValueOf("host", string(argv[1]));
        configFile->SetValueOf("port", string(argv[2]));
        configFile->SetValueOf("channel", string(argv[3]));
        configFile->SetValueOf("nickname", string(argv[4]));
        break;
    default:        // case if wrong number of arguments
        perror("not allowed number of arguments...");
        break;
        return false;
    };
    // serializing config file after editing
    configFile->Save();
    return true;
}
bool IRCBot::Start() {
    // check if bot is configured an if there are four config-attributes
    if (!configFile || configFile->attribute.size() != 4) {
        perror("bot was not configured correctly...");
        return false;
    }
    // get configured values and set nickname and try to connect to server
    nickname = configFile->GetValueOf("nickname");
    if (!ConnectToServer(   configFile->GetValueOf("host"),
                            atoi(configFile->GetValueOf("port").c_str()),
                            configFile->GetValueOf("channel")))
        return false;
    // if everything works fine, start bot loop
    return Loop();
}
void IRCBot::CreateDefaultConfig() {
    if (configFile) {
        cout << "creating default config file..." << endl;
        configFile->AddAttribute("host",        "irc.freenode.net");
        configFile->AddAttribute("port",        "6667");
        configFile->AddAttribute("channel",     "BlubChannel");
        configFile->AddAttribute("nickname",    "BuenyBot");
        configFile->Save();
        cout << "done!" << endl;
    }
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
        IRCLibrary::Send("QUIT :bye!");
        return false;
    } else if ((pos = messageObject.Find("-show_log")) != -1) {
        ShowLog(messageObject);
    } else if ((pos = messageObject.Find("-show_lastseen")) != -1) {
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
void IRCBot::ShowLog(IRCMessageObject &messageObject) {
    string log = GetLog();
    if (log.empty()) {
        CheckMessageAndSendResponse(messageObject, "Empty Log!");
        return;
    }
    size_t pos1 = 0, pos2;
    while ((pos2 = log.find("\n", pos1)) != string::npos) {
        CheckMessageAndSendResponse(messageObject, log.substr(pos1, pos2 - pos1));
        cout << log.substr(pos1, pos2 - pos1) << endl;
        pos1 = pos2 + 1;
    }

}
string IRCBot::GetLog() {
    return LogDatabase::GetLog();
}
string IRCBot::GetLastSeen(string nickname) {
    string lastseen = LogDatabase::LastSeen(nickname);
    if (lastseen.empty())
        return (nickname + " was never logged...");
    return lastseen;
}
