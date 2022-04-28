
#include "renderer/renderer.h"

#include "objects/instancedtrianglemesh.h"
#include "managers/meshmanager.h"
#include "view/view.h"
#include "materials/materialtypeheaders.h"
#include "engine/engine.h"

namespace lightman
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
        std::unordered_map<std::string, InstancedTriangleMesh*> imeshes= view->GetScene()->GetInstanceMeshes();
        std::unordered_map<std::string, InstancedTriangleMesh*>::iterator iter = imeshes.begin();
        InstancedTriangleMesh * currentMesh = nullptr;
        while (iter!=imeshes.end())
        {
            currentMesh = iter->second;

            // This will check if any operation shoule be applied before drawing the mesh
            // Usually this would return quickly after the first loop.
            currentMesh->PrepareForRasterGPU(this);

            // init/update program in case user set new MaterialInstance
            if(currentMesh->IsNeedToUpdateProgram())
            {
                Material::MaterialType type = currentMesh->GetMaterialInstance()->GetMaterial()->getMaterialType();
                uint32_t index = currentMesh->GetProgramIndexBySupportedVertexAttribute();
                HwProgram* program = GetProgram(type, index);
                currentMesh->UpdateProgram(program);
            }

            bool cameraInfoNeedToUpdate = true; // TODO replace
            if(cameraInfoNeedToUpdate)
            {
                Matrix4X4 pvMatrix = view->getCamera()->GetProjectionViewMatrix();
                currentMesh->setPVTransform(pvMatrix);
            }

            currentMesh->Draw();

            iter++;
        } 
    }
    HwProgram* GPURenderer::GetProgram(Material::MaterialType type, uint32_t index)
    {
        if(type >= Material::MaterialType::MAX_MATERIALTYPE_COUNT || type < 0 )
            assert(0);

        HwProgram* result = nullptr;

        auto& typedPrograms = m_programs[type];
        auto iter = typedPrograms.find(index);
        if (iter == typedPrograms.end())
        {
            std::string vertexShaderString;
            std::string fragmentShaderString;
            switch (type)
            {
            case Material::MaterialType::MATTE :
                vertexShaderString = MatteMaterial::CreateVertexShaderString(index);
                fragmentShaderString = MatteMaterial::CreateFragmentShaderString(index);
                break;
            
            default:
                break;
            }

            result = Engine::GetInstance()->GetDriver()->createProgram(vertexShaderString, fragmentShaderString);
            typedPrograms.insert({index, result});
        }
        else
            result = iter->second;

        return result;
        
    }
}
