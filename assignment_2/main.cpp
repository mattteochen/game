#include "game.h"

int main (int agrc, char *argv[])
{
    game::Game  new_game("../config.txt");
    new_game.run();

    return 0;
}
