#ifndef _LIGHTMAN_SWAPCHAIN_H
#define _LIGHTMAN_SWAPCHAIN_H

#include "backend/driverbase.h"

namespace lightman
{
    class Engine;
    class SwapChain
    {
    public:
        void makeCurrent();
        void Commit();
    friend class Engine;
    protected:
        SwapChain(void* nativeWindow);
        ~SwapChain();
    private:
        backend::HwSwapChain* m_swapchain;
    };
}
#endif // _LIGHTMAN_SWAPCHAIN_H