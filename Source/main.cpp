#include "raylib_wrapper.h" //TODO: remove
#include "game.h"

int main(void)
{    
    //TODO: wrap in try / catch, handle all exceptions you throw


    try
    {
        Game game{};
        game.run();
    }
    catch (const std::runtime_error& e)
    {
        TraceLog(LOG_ERROR, "Runtime error: %s", e.what());
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        TraceLog(LOG_ERROR, "Exception: %s", e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        TraceLog(LOG_ERROR, "Unknown error occurred");
        return EXIT_FAILURE;
    }


    return 0;
}