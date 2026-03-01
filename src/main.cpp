#include "game.hpp"

int main()
{
    Game game;
    game.Init({ 800, 600 }, "Hurry Chess");

    if(!game.LoadAssets())
        return -1;

    game.Run();
    return 0;
}