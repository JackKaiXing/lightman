#ifndef _LIGHTMAN_COCOAOPENGLPLATFORM_H
#define _LIGHTMAN_COCOAOPENGLPLATFORM_H

#include "backend/opengl/openglplatform.h"

namespace lightman
{
    namespace backend
    {
        struct CocoaOpenGLPlatformImpl;
        // ----------------------------------------------------------------------------
        class CocoaOpenGLPlatform final : public OpenGLPlatform
        {
        public:
            backend::Driver* CreateDriver() noexcept override;
            void DestroyDriver(backend::Driver* targetDriver) noexcept override;
            CocoaOpenGLPlatform();
            ~CocoaOpenGLPlatform();
            Platform::SwapChain* CreateSwainChain(void* nativeWindow) override;
            void DestroySwapChain(Platform::SwapChain* sc) override;
            void MakeCurrent(Platform::SwapChain* drawswapchain) override;
            void Commit(Platform::SwapChain* drawswapchain) override;
        private:
            CocoaOpenGLPlatformImpl * m_cocoaImpl;
        };
    }
}

#endif // _LIGHTMAN_COCOAOPENGLPLATFORM_H
