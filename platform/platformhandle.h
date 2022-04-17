
#ifndef _LIGHTMAN_PHANDLE_H
#define _LIGHTMAN_PHANDLE_H

#include <functional>

namespace lightman
{
    extern "C" {
        void* GetNativeWindow();
        int MainWindow(int argc, const char* argv[], std::function<void()> setup, std::function<void()> render, 
            unsigned int w, unsigned int h);
    }
}
#endif //_LIGHTMAN_PHANDLE_H
