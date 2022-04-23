#ifndef _LIGHTMAN_SWAPCHAIN_H
#define _LIGHTMAN_SWAPCHAIN_H

#include "backend/driverbase.h"

namespace lightman
{
    class Engine;
    class SwapChain
    {
    public:
        SwapChain(backend::HwSwapChain* swapchain);
        ~SwapChain();
        void makeCurrent();
        void Commit();
    private:
        backend::HwSwapChain* m_swapchain;
    };
}
#endif // _LIGHTMAN_SWAPCHAIN_H