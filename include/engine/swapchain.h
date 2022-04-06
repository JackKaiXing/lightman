#ifndef _LIGHTMAN_SWAPCHAIN_H
#define _LIGHTMAN_SWAPCHAIN_H

#include "backend/driverbase.h"

namespace lightman
{
    namespace engine
    {
        class Engine;
        class SwapChain
        {
        public:
            SwapChain(Engine* engine, backend::HwSwapChain* swapchain);
            ~SwapChain();
            void makeCurrent();
        private:
            backend::HwSwapChain* m_swapchain;
            Engine* m_engine;
        };
    }
}
#endif // _LIGHTMAN_SWAPCHAIN_H