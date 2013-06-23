#include "IRCBot.h"

int main(int argc, char **argv) {

    IRCBot bot;
    // try to configure bot
    if (!bot.Configure(argc, argv, "config.cfg")) {
        perror("failed to configure bot");
        return -1;
    }
    // start bot loop if configuraton was successful
    bot.Start();

    return 0;
}
