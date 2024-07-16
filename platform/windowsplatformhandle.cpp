
#include <assert.h>

#include "platformhandle.h"
#include "..\include\backend\windows\windowsopenglplatform.h"

WinSystemClass* WinSystemClass::GetWinSystemClass()
{
    static WinSystemClass* GDelegateSingleton;
    if (GDelegateSingleton == nullptr)
    {
        GDelegateSingleton = new WinSystemClass();
    }
    return GDelegateSingleton;
}

namespace lightmangui
{
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
        return WinSystemClass::GetWinSystemClass()->m_hwnd;
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
        WinSystemClass::GetWinSystemClass()->m_hinstance = GetModuleHandle(NULL);
        
        // https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
        MSG msg = { 0 };
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WndProc;
        wc.hInstance = WinSystemClass::GetWinSystemClass()->m_hinstance;
        wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
        wc.lpszClassName = TEXT("LightMan");
        wc.style = CS_OWNDC;
        if (!RegisterClass(&wc))
            return 1;

        WinSystemClass::GetWinSystemClass()->m_hwnd =
            CreateWindowW(TEXT(L"LightMan")/*wc.lpszClassName*/, TEXT(L"LightMan"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, w, h, 0, 0, wc.hInstance, 0);

        if (WinSystemClass::GetWinSystemClass()->m_hwnd == nullptr)
        {
            assert(0);
        }

        setup();

        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            DispatchMessage(&msg);
            render();
        }

        return 0;
    }
}
