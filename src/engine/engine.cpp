
#include "engine/engine.h"
#include "managers/meshmanager.h"

namespace lightman
{
    Engine * g_engine = nullptr;

    Engine * Engine::Create(backend::BackendType backend)
    {
        if (g_engine == nullptr)
        {
            g_engine = new Engine(backend);
        }
        return g_engine;
    }

    void Engine::DestroyInstance()
    {
        MeshManager::DetoryInstance();
    }
    
    Engine* Engine::GetInstance()
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

        m_defaultRT = m_driver->createDefaultRenderTarget();
    }
    Engine::~Engine()
    {
        // TODO other destory
    }
    SwapChain* Engine::CreateSwapChain(void* nativeWindow)
    {
        return new SwapChain(m_driver->CreateSwapChain(nativeWindow));
    }
    Renderer* Engine::CreateRender(uint32_t width, uint32_t height, RenderType type)
    {
        switch (type)
        {
        case RenderType::RASTER_GPU:
            {
                return new GPURenderer(width, height);
            }
            break;
        
        default:
            {
                return nullptr;
            }
            break;
        }
    }
    void Engine::DestroyRender(Renderer* render)
    {
        delete render;
    }
}
