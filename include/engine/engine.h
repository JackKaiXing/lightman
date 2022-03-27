#ifndef _LIGHTMAN_ENGINE_H
#define _LIGHTMAN_ENGINE_H

#include <cstdint>
#include <array>

#include "backend/platform.h"
#include "backend/driverbase.h"
#include "renderer/renderer.h"

namespace lightman
{
    namespace engine
    {
        
        class Engine
        {
        public:
            static Engine* Create();

            Engine(Engine const&) = delete;
            Engine(Engine&&) = delete;
            Engine& operator=(Engine const&) = delete;
            Engine& operator=(Engine&&) = delete;
        
        protected:
        Engine() noexcept = default;
        ~Engine() = default;

        private:
            backend::Platform * m_platform;
            backend::Driver * m_driver;
            renderer::Renderer * m_currentRenderer;
            std::array<renderer::Renderer *, renderer::MAX_RENDERER_TYPE_COUNT> m_renderers;
        };
    }
}
#endif // _LIGHTMAN_PLATFORM_H