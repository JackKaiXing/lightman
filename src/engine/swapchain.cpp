
#include "engine/swapchain.h"
#include "engine/engine.h"

namespace lightman
{
    SwapChain::SwapChain(backend::HwSwapChain* swapchain)
    {
        m_swapchain = swapchain;
    }
    SwapChain::~SwapChain()
    {
        m_swapchain = nullptr;
    }
    void SwapChain::makeCurrent()
    {
        Engine::GetInstance()->GetDriver()->makeCurrent(m_swapchain);
    }
    void SwapChain::Commit()
    {
        Engine::GetInstance()->GetDriver()->Commit(m_swapchain);
    }
}
