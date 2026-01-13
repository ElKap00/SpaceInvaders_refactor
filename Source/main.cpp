#include "raylib_wrapper.h" //TODO: remove
#include "game.h"

int main(void)
{    
    //TODO: wrap in try / catch, handle all exceptions you throw
    Game game{};
	//consider: game.run(); method that contains the loop
    while (!WindowShouldClose())
    {
        game.update();
        game.draw();
    }

    return 0;
}