
#include "renderer/renderer.h"

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
    GPURenderer::GPURenderer(uint32_t width, uint32_t height)
    {
        m_colorTex = Engine::GetInstance()->GetDriver()->createTexture(backend::SamplerType::SAMPLER_2D, 1,
            backend::TextureFormat::RGBA8,1,width,height,1,backend::TextureUsage::SAMPLEABLE);
        backend::MRT colorMRT({m_colorTex,0});
        
        HwTexture* depthTex = Engine::GetInstance()->GetDriver()->createTexture(backend::SamplerType::SAMPLER_2D, 1,
            backend::TextureFormat::DEPTH32F,1,width,height,1,backend::TextureUsage::DEPTH_ATTACHMENT);
        backend::TargetBufferInfo depthInfo(depthTex,0);

        m_mrt = Engine::GetInstance()->GetDriver()->createRenderTarget(
            (backend::TargetBufferFlags)((uint32_t)backend::TargetBufferFlags::COLOR0 | (uint32_t)backend::TargetBufferFlags::DEPTH),
            width,height,1,colorMRT,depthInfo,NULL);
        
        m_mrtPP.clearColor = {0.0, 1.0, 1.0, 1.0};
        m_mrtPP.flags.clear = (backend::TargetBufferFlags)((uint32_t)backend::TargetBufferFlags::COLOR0 | (uint32_t)backend::TargetBufferFlags::DEPTH);
        m_mrtPP.viewport = lightman::Viewport(0,0,width,height);

        // TODO Remove to config
        std::string vertexShaderString = "#version 330 core \n \
            layout(location = 0) in vec3 position; \n \
            out vec2 uv; \n \
            void main() \n \
            { \n \
                gl_Position = vec4(position, 1.0f); \n \
                uv = position.xy / 2.0 + 0.5; \n \
            }";
        std::string fragmentShaderString = "#version 330 core \n \
            uniform sampler2D geometryTex; \n \
            out vec4 color; \n \
            in vec2 uv; \n \
            void main() \n \
            { \n \
                color.rgb = texture(geometryTex, uv).rgb; \n \
                //color.rgb = vec3(uv.x, uv.y, 1.0); \n \
                color.a = 1.0; \n \
            }";
        m_postprocessing_fxaa = Engine::GetInstance()->GetDriver()->createProgram(vertexShaderString, fragmentShaderString);
        std::vector<unsigned int> triIndexs = {0, 1, 2, 0, 2, 3};
        std::vector<float> points = 
            {-1.0, -1.0, 0.0,
              1.0, -1.0, 0.0,
              1.0,  1.0, 0.0,
             -1.0,  1.0, 0.0,
            };
        m_quad = new TriangleMesh(triIndexs, points);
        m_quad->PrepareForRasterGPU();
    }
    GPURenderer::~GPURenderer()
    {
        // TODO DESTORY Resources
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
        // Draw Into Custom FBO
        Engine::GetInstance()->GetDriver()->beginRenderPass(m_mrt, m_mrtPP);
        
        // processing vertex arrays for all imesh for current scene
        std::unordered_map<std::string, InstancedTriangleMesh*> imeshes= view->GetScene()->GetInstanceMeshes();
        std::unordered_map<std::string, InstancedTriangleMesh*>::iterator iter = imeshes.begin();
        InstancedTriangleMesh * currentMesh = nullptr;
        while (iter!=imeshes.end())
        {
            currentMesh = iter->second;

            // This will check if any operation shoule be applied before drawing the mesh
            // Usually this would return quickly after the first loop.
            currentMesh->PrepareForRasterGPU();

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
        
        // End Draw Into Custom FBO
        Engine::GetInstance()->GetDriver()->endRenderPass();

        // post processing pass
        Engine::GetInstance()->GetDriver()->bindSamplers(0, m_colorTex);
        m_quad->Draw(m_postprocessing_fxaa);
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
