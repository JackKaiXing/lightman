
#include "backend/opengl/opengldriver.h"

#include <iostream>

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        backend::Driver* OpengGLDriver::create(backend::OpenGLPlatform* platform, void* sharedGLContext) noexcept
        {
            return new OpengGLDriver(platform, sharedGLContext);
        }
        OpengGLDriver::OpengGLDriver(backend::OpenGLPlatform* platform, void* sharedGLContext)
        {
            m_platfrom = platform;
        }
        backend::HwSwapChain* OpengGLDriver::createSwapChain(void* nativeWindow)
        {
            backend::HwSwapChain* swapchain = new backend::HwSwapChain();
            swapchain->swapchain = m_platfrom->CreateSwainChain(nativeWindow);
            return swapchain;
        }
    }
}