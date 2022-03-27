
#include "engine/engine.h"

namespace lightman
{
    namespace engine
    {
        Engine * g_engine = nullptr;

        Engine * Engine::Create()
        {
            if (g_engine)
            {
               return g_engine;
            }
            return new Engine();
        }
    }
}