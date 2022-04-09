
#include "engine/engine.h"
#include "managers/meshmanager.h"

namespace lightman
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

    void Engine::DestroyInstance()
    {
        MeshManager::DetoryInstance();
    }
    
    Engine* GetInstance()
    {
        if (g_engine)
        {
            return g_engine;
        }else{
            std::cout << "Please create Engine before, GetInstance!\n" << std::endl;
            assert(0);
        }
    }
    // ----------------------------------------------------------------------------

    Engine::Engine(backend::BackendType backend) noexcept
    {
        // TODO: multi-threading for rendering context
        m_platform = backend::Platform::Create(backend);
        if (m_platform==nullptr)
            assert(0);
        m_driver = m_platform->CreateDriver();
    }
    SwapChain* Engine::CreateSwapChain(void* nativeWindow)
    {
        return new SwapChain(this, m_driver->CreateSwapChain(nativeWindow));
    }
    Renderer* Engine::CreateRender(RenderType type)
    {
        switch (type)
        {
        case RenderType::RASTER_GPU:
            {
                return new GPURenderer(this);
            }
            break;
        
        default:
            {
                return nullptr;
            }
            break;
        }
    }
}
