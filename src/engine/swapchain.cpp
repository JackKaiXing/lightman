
#include "engine/swapchain.h"
#include "engine/engine.h"

namespace lightman
{
    SwapChain::SwapChain(void* nativeWindow)
    {
        m_swapchain =  Engine::GetInstance()->GetDriver()->CreateSwapChain(nativeWindow);
    }
    SwapChain::~SwapChain()
    {
        if (m_swapchain)
            Engine::GetInstance()->GetDriver()->DestroySwapChain(m_swapchain);
        m_swapchain = nullptr;
    }
    void SwapChain::makeCurrent()
    {
        Engine::GetInstance()->GetDriver()->MakeCurrent(m_swapchain);
    }
    void SwapChain::Commit()
    {
        Engine::GetInstance()->GetDriver()->Commit(m_swapchain);
    }
}
