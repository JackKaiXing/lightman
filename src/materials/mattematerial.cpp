
#include "materials/mattematrial.h"
#include "engine/engine.h"
#include "materials/shader.h"

#include <sstream>

namespace lightman
{
    MatteMaterial::MatteMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    MatteMaterial::MatteMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd) 
        : Material(name, bump, emission)
    {
    }
    MatteMaterial::~MatteMaterial()
    {
    }
    bool MatteMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        std::vector<UniformDefine> uDefines;
        // get shared uniform defines
        ShaderString::GetSharedBlockInfo(uDefines);

        // add custom uniform defines
        // TO DO PARSE TEXTURE NODES TO GET UDEFINES
        
        // Init Block Infos
        InitUniformBlockInfo(uDefines);

        // update defaut materialInstance
        UpdateDefaultMaterialInstance();

        // TODO UPDATE MaterialInstance about uDefines

        // Update shaders
        backend::UniformBlockInfo bInfos = ShaderString::GetUniformBufferInfo();
        
        std::string vertexShaderString = ShaderString::GetVertexAttribute();
        vertexShaderString += ShaderString::CreateBlockInfo(uDefines, bInfos.at(0));
        vertexShaderString += ShaderString::GetDisneyVertexShader();

        std::string fragmentShaderString = ShaderString::GetDisneyFragmentShader("");

        // Update Program
        m_program = Engine::GetInstance()->GetDriver()->createProgram(
            vertexShaderString, fragmentShaderString, bInfos);

        m_isRasterGPUNeedUpdate = true;
        return true;
    }
    void MatteMaterial::SetKd(const Texture* kd)
    {
        m_kd = kd;
    }
} // namespace lightman
