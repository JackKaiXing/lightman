
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


            PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
                PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
                32,                   // Colordepth of the framebuffer.
                0, 0, 0, 0, 0, 0,
                0,
                0,
                0,
                0, 0, 0, 0,
                24,                   // Number of bits for the depthbuffer
                8,                    // Number of bits for the stencilbuffer
                0,                    // Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0,
                0, 0, 0
            };

            HDC ourWindowHandleToDeviceContext = GetDC(WinSystemClass::GetWinSystemClass()->m_hwnd);
            WinSystemClass::GetWinSystemClass()->m_windwosHDC = ourWindowHandleToDeviceContext;

            int letWindowsChooseThisPixelFormat;
            letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
            SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

            HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
            wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

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
            SwapBuffers(WinSystemClass::GetWinSystemClass()->m_windwosHDC);
        }
    }
}