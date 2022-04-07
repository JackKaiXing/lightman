
#include "engine/swapchain.h"
#include "engine/engine.h"

namespace lightman
{
    namespace engine
    {
        SwapChain::SwapChain(Engine* engine, backend::HwSwapChain* swapchain)
        {
            m_swapchain = swapchain;
            m_engine = engine;
        }
        SwapChain::~SwapChain()
        {
            m_swapchain = nullptr;
            m_engine = nullptr;
        }
        void SwapChain::makeCurrent()
        {
            m_engine->GetDriver()->makeCurrent(m_swapchain);
        }
        void SwapChain::Commit()
        {
            m_engine->GetDriver()->Commit(m_swapchain);
        }
    }
}
