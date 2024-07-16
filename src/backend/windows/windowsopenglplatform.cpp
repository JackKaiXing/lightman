
#include "backend/windows/windowsopenglplatform.h"

#include "Windows.h"
#include <bluegl/BlueGL.h>

#include "backend/opengl/opengldriver.h"

namespace lightman
{
    namespace backend
    {
        struct WindowsGLSwapChain : public Platform::SwapChain
        {
            WindowsGLSwapChain(HWND inView);
            ~WindowsGLSwapChain() noexcept;

            HWND view;
        };
        WindowsGLSwapChain::WindowsGLSwapChain(HWND inView)
            : view(inView)
        {

        }
        WindowsGLSwapChain::~WindowsGLSwapChain() noexcept
        {
            view = nullptr;
        }

        WindowsOpenGLPlatform::WindowsOpenGLPlatform()
        {

        }
        WindowsOpenGLPlatform::~WindowsOpenGLPlatform()
        {

        }

        backend::Driver* WindowsOpenGLPlatform::CreateDriver() noexcept
        {
            // https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
            int result = bluegl::bind();
            assert(!result);

            return OpenGLDriver::create(this, nullptr);
        }

        void WindowsOpenGLPlatform::DestroyDriver(backend::Driver* targetDriver) noexcept
        {

        }
        Platform::SwapChain* WindowsOpenGLPlatform::CreateSwainChain(void* nativeWindow)
        {
            //return new WindowsGLSwapChain(nativeWindow);
            return nullptr;
        }
        void WindowsOpenGLPlatform::DestroySwapChain(Platform::SwapChain* sc)
        {

        }
        void WindowsOpenGLPlatform::MakeCurrent(Platform::SwapChain* drawswapchain)
        {

        }
        void WindowsOpenGLPlatform::Commit(Platform::SwapChain* drawswapchain)
        {

        }
    }
}