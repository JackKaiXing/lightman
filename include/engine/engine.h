#ifndef _LIGHTMAN_ENGINE_H
#define _LIGHTMAN_ENGINE_H

#include <cstdint>
#include <array>
#include <iostream>

#include "backend/driverbase.h"
#include "backend/platform.h"
#include "engine/swapchain.h"
#include "renderer/renderer.h"

namespace lightman
{
    class Engine
    {
    public:
        SwapChain* CreateSwapChain(void* nativeWindow);
        Renderer* CreateRender(uint32_t width, uint32_t height, RenderType type = RenderType::RASTER_GPU);
    public:
        static Engine* Create(backend::BackendType backend = backend::BackendType::OPENGL); //  TODO: be removed out of this file
        static Engine* GetInstance();
        static void DestroyInstance();

        Engine(Engine const&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine const&) = delete;
        Engine& operator=(Engine&&) = delete;
    
        backend::Driver * GetDriver() {return m_driver;};
        backend::HwRenderTarget* GetDefaultRenderTarget(){return m_defaultRT;};
    protected:
        Engine(backend::BackendType backend) noexcept;
        ~Engine() = default;

    private:
        backend::Platform * m_platform;
        backend::Driver * m_driver;
        backend::HwRenderTarget* m_defaultRT;
    };
}
#endif // _LIGHTMAN_PLATFORM_H
