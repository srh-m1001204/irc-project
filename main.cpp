#include "IRCBot.h"

int main(int argc, char **argv) {
    IRCBot bot("CrazyBot");
    if (!bot.ConnectToServer("irc.freenode.net", 6667, "CrazyChannel"))
        exit(1);
    getchar();
    bot.Start();

    return 0;
}
