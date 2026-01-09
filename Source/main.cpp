#include "raylib_wrapper.h"
#include "game.h"

int main(void)
{    
    Game game{};

    while (!WindowShouldClose())
    {
        game.update();
        game.draw();
    }

    return 0;
}