
#include "materials/velvetmaterial.h"
#include "materials/shader.h"

namespace lightman
{
    VelvetMaterial::VelvetMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    VelvetMaterial::VelvetMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd) 
        : Material(name, bump, emission)
    {
    }
    VelvetMaterial::~VelvetMaterial()
    {
    }
    bool VelvetMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        // prepare uniform buffer with custom texture nodes
        std::vector<const Texture*> customTextures;
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
    void VelvetMaterial::SetKd(const Texture* kd)
    {
        m_kd = kd;
    }
} // namespace lightman
