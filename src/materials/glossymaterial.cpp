
#include "materials/glossymaterial.h"
#include "materials/shader.h"

namespace lightman
{
    GlossyMaterial::GlossyMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    GlossyMaterial::GlossyMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd) 
        : Material(name,NULL,NULL)
    {
    }
    GlossyMaterial::~GlossyMaterial()
    {
    }
    bool GlossyMaterial::PrepareForRasterGPU()
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
    void GlossyMaterial::SetKd(const Texture* kd)
    {
        m_kd = kd;
    }
} // namespace lightman
