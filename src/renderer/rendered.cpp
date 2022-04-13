
#include "renderer/renderer.h"

#include "objects/instancedtrianglemesh.h"
#include "managers/meshmanager.h"

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
        // GPURenderer is responsible for calling all the backend method.

        // processing vertex arrays for all imesh for current scene
        std::unordered_map<string, InstancedTriangleMesh*> imeshes= view->GetScene()->GetInstanceMeshes();
        std::unordered_map<string, InstancedTriangleMesh*>::iterator iter = imeshes.begin();
        while (iter!=imeshes.end())
        {
            // This will check if any operation shoule be applied before drawing the mesh
            // Usually this would return quickly after the first loop.
            iter->second->PrepareForRasterGPU();

            iter++;
        } 
    }
}