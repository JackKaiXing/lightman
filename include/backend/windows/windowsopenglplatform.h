#ifndef _LIGHTMAN_WINDOWSOPENGLPLATFORM_H
#define _LIGHTMAN_WINDOWSOPENGLPLATFORM_H

#include "backend/opengl/openglplatform.h"

namespace lightman
{
    namespace backend
    {
        struct WindowsOpenGLPlatformImpl;
        // ----------------------------------------------------------------------------
        class WindowsOpenGLPlatform final : public OpenGLPlatform
        {
        public:
            backend::Driver* CreateDriver() noexcept override;
            void DestroyDriver(backend::Driver* targetDriver) noexcept override;
            WindowsOpenGLPlatform();
            ~WindowsOpenGLPlatform();
            Platform::SwapChain* CreateSwainChain(void* nativeWindow) override;
            void DestroySwapChain(Platform::SwapChain* sc) override;
            void MakeCurrent(Platform::SwapChain* drawswapchain) override;
            void Commit(Platform::SwapChain* drawswapchain) override;
        private:
            WindowsOpenGLPlatformImpl* m_cocoaImpl;
        };
    }
}

#endif // _LIGHTMAN_WINDOWSOPENGLPLATFORM_H
