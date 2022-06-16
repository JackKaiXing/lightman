
#include "renderer/renderer.h"

#include "managers/meshmanager.h"
#include "view/view.h"
#include "materials/materialtypeheaders.h"
#include "engine/engine.h"
#include "utils/fileoperator.h"

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
        Driver* targetDriver = Engine::GetInstance()->GetDriver();
        
        m_colorTex = targetDriver->CreateTexture(backend::SamplerType::SAMPLER_2D, 1,
            backend::TextureFormat::RGBA8,1,width,height,1,backend::TextureUsage::SAMPLEABLE);
        backend::MRT colorMRT({m_colorTex,0});
        
        m_depthTex = targetDriver->CreateTexture(backend::SamplerType::SAMPLER_2D, 1,
            backend::TextureFormat::DEPTH32F,1,width,height,1,backend::TextureUsage::DEPTH_ATTACHMENT);
        backend::TargetBufferInfo depthInfo(m_depthTex,0);

        m_mrt = targetDriver->CreateRenderTarget(
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
        std::string fragmentShaderString = "#version 330 core \n";
        std::string fxaaShader = FileOperator::GetFileAsString("/Users/XK/Desktop/Projects/lightman/shaders.h/fxaa.fs");
        fragmentShaderString += fxaaShader;
        fragmentShaderString += "uniform sampler2D geometryTex; \n \
        out vec4 color; \n \
        in vec2 uv; \n \
        void main() \n \
            { \n \
                // First, compute an exact upper bound for the area we need to sample from. \n \
                // The render target may be larger than the viewport that was used for scene \n \
                // rendering, so we cannot rely on the wrap mode alone. \n \
                highp vec2 fboSize = vec2(textureSize(geometryTex, 0)); \n \
                highp vec2 invSize = 1.0 / fboSize;  \n \
                highp vec2 halfTexel = 0.5 * invSize;  \n \
                highp vec2 viewportSize = fboSize; // TODO get viewport size \n \
                // The clamp needs to be over-aggressive by a half-texel due to bilinear sampling. \n \
                highp vec2 excessSize = 0.5 + fboSize - viewportSize; \n \
                highp vec2 upperBound = 1.0 - excessSize * invSize; \n \
                // Next, compute the coordinates of the texel center and its bounding box. \n \
                // There is no need to clamp the min corner since the wrap mode will do \n \
                // it automatically. \n \
                // variable_vertex is already interpolated to pixel center by the GPU  \n \
                highp vec2 texelCenter = min(uv, upperBound);  \n \
                highp vec2 texelMaxCorner = min(uv + halfTexel, upperBound);  \n \
                highp vec2 texelMinCorner = uv - halfTexel;  \n \
                color = fxaa(  \n \
                        texelCenter,  \n \
                        vec4(texelMinCorner, texelMaxCorner),  \n \
                        geometryTex,  \n \
                        invSize,             // FxaaFloat4 fxaaConsoleRcpFrameOpt,  \n \
                        2.0 * invSize,       // FxaaFloat4 fxaaConsoleRcpFrameOpt2,  \n \
                        8.0,                 // FxaaFloat fxaaConsoleEdgeSharpness,  \n \
        #if defined(G3D_FXAA_PATCHES) && G3D_FXAA_PATCHES == 1  \n \
                        0.08,                // FxaaFloat fxaaConsoleEdgeThreshold,  \n \
        #else  \n \
                        0.125,               // FxaaFloat fxaaConsoleEdgeThreshold,  \n \
        #endif  \n \
                        0.04                 // FxaaFloat fxaaConsoleEdgeThresholdMin  \n \
                );  \n \
                color.a = 1.0; \n \
            }";
        UniformBlockInfo ubInfo;
        m_postprocessing_fxaa = targetDriver->CreateProgram(vertexShaderString, fragmentShaderString, ubInfo);
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
        lightman::backend::Driver * driver = Engine::GetInstance()->GetDriver();
        if (m_colorTex)
            driver->DestroyTexture(m_colorTex);
        if (m_depthTex)
            driver->DestroyTexture(m_depthTex);
        if (m_postprocessing_fxaa)
            driver->DestroyProgram(m_postprocessing_fxaa);
        if (m_quad)
            delete m_quad;
        if (m_mrt)
            driver->DestroyRenderTarget(m_mrt);
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
        // ---------------------------------------------Draw Into Custom FBO-----------------------------------------
        Driver* targetDriver = Engine::GetInstance()->GetDriver();
        targetDriver->BeginRenderPass(m_mrt, m_mrtPP);
        
        // processing camera moving
        Matrix4X4 pvMatrix;
        Vector3 cameraPos;
        bool isNeedToUpdatePVMatrix = false;
        if(! view->getCamera()->IsCameraMovingConsumed())
        {
            pvMatrix = view->getCamera()->GetProjectionViewMatrix();
            view->getCamera()->SetCameraMovingConsumed();
            cameraPos = view->getCamera()->GetWorldPosition();
            isNeedToUpdatePVMatrix = true;
        }
        
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
            if(currentMesh->hasMaterial())
            {
                // TODO use default material
            }

            if(isNeedToUpdatePVMatrix)
            {
                currentMesh->setPVTransform(pvMatrix, cameraPos);
            }

            currentMesh->Draw();

            iter++;
        }
        
        Engine::GetInstance()->GetDriver()->endRenderPass();
        // ---------------------------------------------End Draw Into Custom FBO-----------------------------------------

        // ---------------------------------------------Post Processing, Start Default FBO-------------------------------
        targetDriver->BeginRenderPass(Engine::GetInstance()->GetDefaultRenderTarget(), m_mrtPP);
        targetDriver->BindSamplers(0, m_colorTex);
        m_quad->Draw(m_postprocessing_fxaa);
        Engine::GetInstance()->GetDriver()->endRenderPass();
        // ---------------------------------------------Post Processing, End Default FBO---------------------------------
    }
}
