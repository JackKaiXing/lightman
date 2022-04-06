
#include "engine/engine.h"

namespace lightman
{
    namespace engine
    {
        Engine * g_engine = nullptr;

        Engine * Engine::Create(backend::BackendType backend)
        {
            if (g_engine)
            {
               return g_engine;
            }
            return new Engine(backend);
        }

        Engine::Engine(backend::BackendType backend) noexcept
        {
            // TODO: multi-threading for rendering context
            m_platform = backend::Platform::Create(backend);
            if (m_platform==nullptr)
                assert(0);
            m_driver = m_platform->CreateDriver();
        }
        engine::SwapChain* Engine::CreateSwapChain(void* nativeWindow)
        {
            return new engine::SwapChain(m_driver->createSwapChain(nativeWindow));
        }
    }
}
