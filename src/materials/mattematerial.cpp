
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
        // get blender workbench uniform defines
        ShaderString::GetBlenderBlockInfo(uDefines);
        // add custom uniform defines
        if (m_bump)
            m_bump->GetBlockInfo(uDefines);
        if (m_emission)
            m_emission->GetBlockInfo(uDefines);
        if (m_kd)
            m_kd->GetBlockInfo(uDefines);
        
        // Init Block Infos
        InitUniformBlockInfo(uDefines);

        // update defaut materialInstance
        UpdateDefaultMaterialInstance();

        // update parameters of materialInstance about uDefines
        if (m_bump)
            ;
        if (m_emission)
            ;
        if (m_kd)
            ;

        // uniform block shader
        backend::UniformBlockInfo bInfos;
        bInfos.at(0) = "targetUniform";
        const std::string UniformShaderBlock = ShaderString::CreateBlockInfo(uDefines, bInfos.at(0));

        // vertex shader
        std::string vertexShaderString = ShaderString::GetVertexAttribute();
        vertexShaderString += UniformShaderBlock;
        vertexShaderString += ShaderString::GetBlenderVertexShader();

        // fragment shader
        std::string fragmentShaderString = ShaderString::GetFragmentShaderHead();
        fragmentShaderString += UniformShaderBlock;
        std::string UpdateUserMaterialParameters;
        if (m_bump)
            ;
        if (m_emission)
            ;
        if (m_kd)
        {
            backend::UniformType type = m_kd->GetShaderString(UpdateUserMaterialParameters);
            UpdateUserMaterialParameters += "paras.baseColor = " + m_kd->GetName() + ";\n";
        }
            
        fragmentShaderString += ShaderString::GetBlenderFragmentShader(UpdateUserMaterialParameters);

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
