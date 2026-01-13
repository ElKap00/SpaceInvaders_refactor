#include "raylib_wrapper.h" //TODO: remove
#include "game.h"

int main(void)
{    
    //TODO: wrap in try / catch, handle all exceptions you throw
    Game game{};

    game.run();

    return 0;
}