
#include <windows.h>

#include "platformhandle.h"

namespace lightmangui
{
    using Callback = std::function<void()>;

    class WinSystemClass
    {
    public:
        HINSTANCE m_hinstance;
        HWND m_hwnd;

        Callback _setup;
        Callback _render;
        Callback _destroy;
    };
    WinSystemClass* GDelegate = nullptr;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        // Check if the window is being closed.
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_CREATE:
        {
           
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;

    }

    void* GetNativeWindow()
    {
        return GDelegate->m_hwnd;
    }

    float GetBackScaleFactor()
    {
        return 1.0;
    }

    int MainWindow(int argc, const char* argv[],
        std::function<void()> setup,
        std::function<void()> render,
        std::function<void()> destory,
        unsigned int w, unsigned int h)
    {
        GDelegate = new WinSystemClass();
        GDelegate->m_hinstance = GetModuleHandle(NULL);
        GDelegate->_setup = setup;
        GDelegate->_render = render;
        GDelegate->_destroy = destory;

        MSG msg = { 0 };
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GDelegate->m_hinstance;
        wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
        wc.lpszClassName = TEXT("LightMan");
        wc.style = CS_OWNDC;
        if (!RegisterClass(&wc))
            return 1;

        GDelegate->m_hwnd =
            CreateWindowW(TEXT(L"LightMan")/*wc.lpszClassName*/, TEXT(L"openglversioncheck"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, w, h, 0, 0, wc.hInstance, 0);

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

        HDC ourWindowHandleToDeviceContext = GetDC(GDelegate->m_hwnd);

        int letWindowsChooseThisPixelFormat;
        letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
        SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

        HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
        wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

        if (GDelegate->m_hwnd == nullptr)
        {
            return 1;
        }

        GDelegate->_setup();

        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            DispatchMessage(&msg);
            GDelegate->_render();
            SwapBuffers(ourWindowHandleToDeviceContext);
        }

        return 0;
    }
}
