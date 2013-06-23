#include "IRCBot.h"

int main(int argc, char **argv) {

    IRCBot bot;
    bot.Configure(argc, argv, "config.cfg");
    bot.Start();

    return 0;
}
