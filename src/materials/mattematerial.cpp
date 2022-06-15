
#include "materials/mattematrial.h"
#include "materials/shader.h"

namespace lightman
{
    MatteMaterial::MatteMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    MatteMaterial::MatteMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd) 
        : Material(name, bump, emission)
    {
    }
    MatteMaterial::~MatteMaterial()
    {
        if(m_kd)
            delete m_kd;
    }
    bool MatteMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        // prepare uniform buffer with custom texture nodes
        std::vector<const Node*> customTextures;
        std::string UpdateUserMaterialParameters;
        if (m_kd)
        {
            customTextures.push_back(m_kd);
            backend::UniformType type = m_kd->GetShaderString(UpdateUserMaterialParameters);
            UpdateUserMaterialParameters += "paras.baseColor = " + m_kd->GetName() + ";\n";
        }
        PrepareForRasterGPUBase(customTextures, UpdateUserMaterialParameters);

        // update default parameters for materialInstance about uDefines
        if (m_bump)
            ;
        if (m_emission)
            ;
        if (m_kd)
            ;

        m_isRasterGPUNeedUpdate = true;
        return true;
    }
    void MatteMaterial::SetKd(const Node* kd)
    {
        m_kd = kd;
    }
} // namespace lightman
