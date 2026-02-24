#include "game.hpp"

int main()
{
    Game game;
    game.Init({ 800, 600 }, "Hurry Chess");
    game.Run();
    return 0;
}