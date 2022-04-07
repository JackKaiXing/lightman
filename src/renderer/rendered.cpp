
#include "renderer/renderer.h"

namespace lightman
{
    namespace renderer
    {
        //----------------------------------------------------------------------------
        Renderer::~Renderer()
        {
            m_swapChain = nullptr;
        }
        //----------------------------------------------------------------------------
        GPURenderer::GPURenderer()
        {
        }
        bool GPURenderer::BeginFrame(SwapChain *swapChain)
        {
            if (swapChain)
            {
                swapChain->makeCurrent();
                m_swapChain = swapChain;
                return true;
            }
            return false;
        }
        void GPURenderer::EndFrame()
        {
            if(m_swapChain)
            {
                m_swapChain->Commit();
                m_swapChain = nullptr;
            }
        }
    }
}