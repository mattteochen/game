#include "game.h"
#include <cstdlib>

int main (int agrc, char *argv[])
{
    while(1)
    {
        game::Game  new_game("../config.txt");
        int ret = new_game.run();
        if (!ret) exit(EXIT_SUCCESS);
    }

    return 0;
}
