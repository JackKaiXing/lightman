
#ifndef _LIGHTMAN_PHANDLE_H
#define _LIGHTMAN_PHANDLE_H

#include <functional>

namespace lightmangui
{
    extern "C" {
        void* GetNativeWindow();
        float GetBackScaleFactor();
        int MainWindow(int argc, const char* argv[], 
            std::function<void()> setup, 
            std::function<void()> render,
            std::function<void()> destory, 
            unsigned int w, unsigned int h);
    }
}
#endif //_LIGHTMAN_PHANDLE_H
