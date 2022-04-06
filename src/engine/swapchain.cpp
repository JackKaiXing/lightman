
#include "engine/swapchain.h"

namespace lightman
{
    namespace engine
    {
        SwapChain::SwapChain(backend::HwSwapChain* swapchain)
        {
            m_swapchain = swapchain;
        }
        SwapChain::~SwapChain()
        {

        }
    }
}