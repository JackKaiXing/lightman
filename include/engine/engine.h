#ifndef _LIGHTMAN_ENGINE_H
#define _LIGHTMAN_ENGINE_H

#include <cstdint>
#include <array>

#include "backend/driverbase.h"
#include "backend/platform.h"
#include "engine/swapchain.h"

namespace lightman
{
    namespace engine
    {
        class Engine
        {
        public:
            engine::SwapChain* CreateSwapChain(void* nativeWindow);
        public:
            static Engine* Create(backend::BackendType backend = backend::BackendType::OPENGL); //  TODO: be removed out of this file

            Engine(Engine const&) = delete;
            Engine(Engine&&) = delete;
            Engine& operator=(Engine const&) = delete;
            Engine& operator=(Engine&&) = delete;
        
        protected:
            Engine(backend::BackendType backend) noexcept;
            ~Engine() = default;

        private:
            backend::Platform * m_platform;
            backend::Driver * m_driver;
        };
    }
}
#endif // _LIGHTMAN_PLATFORM_H