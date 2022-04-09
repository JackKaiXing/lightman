
#include "renderer/renderer.h"

#include "objects/instancedtrianglemesh.h"

namespace lightman
{
    //----------------------------------------------------------------------------
    Renderer::~Renderer()
    {
        m_swapChain = nullptr;
    }
    //----------------------------------------------------------------------------
    GPURenderer::GPURenderer(Engine * engine)
    {
        m_Engine = engine;
    }
    GPURenderer::~GPURenderer()
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
    void GPURenderer::RenderFrame(View* view)
    {
        // GPURender is responsible for calling all the backend method.
        std::unordered_map<string, InstancedTriangleMesh*> imeshes= view->GetScene()->GetInstanceMeshes();
        std::unordered_map<string, InstancedTriangleMesh*>::iterator iter = imeshes.begin();
        while (iter!=imeshes.end())
        {
            iter++;
        } 
    }
}