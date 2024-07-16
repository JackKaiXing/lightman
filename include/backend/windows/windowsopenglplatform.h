#ifndef _LIGHTMAN_WINDOWSOPENGLPLATFORM_H
#define _LIGHTMAN_WINDOWSOPENGLPLATFORM_H

#include <windows.h>

#include "backend/opengl/openglplatform.h"

// TODO, may be there is a better soution
// maybe move this to be shared by directx, todo later
class WinSystemClass
{
public:
    static WinSystemClass* GetWinSystemClass();
protected:
    WinSystemClass()
        : m_hinstance(0), m_hwnd(0), m_windwosHDC(NULL)
    {
    };
    
public:
    HINSTANCE m_hinstance;
    HWND m_hwnd;
    HDC m_windwosHDC; // this is what is needed
};

namespace lightman
{
    namespace backend
    {
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
        };
    }
}

#endif // _LIGHTMAN_WINDOWSOPENGLPLATFORM_H
