
#include "materials/glossycoatingmaterial.h"
#include "materials/shader.h"

namespace lightman
{
    GlossyCoatingMaterial::GlossyCoatingMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    GlossyCoatingMaterial::GlossyCoatingMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd) 
        : Material(name, bump, emission)
    {
    }
    GlossyCoatingMaterial::~GlossyCoatingMaterial()
    {
    }
    bool GlossyCoatingMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        // prepare uniform buffer with custom texture nodes
        std::vector<const Texture*> customTextures;
        std::string UpdateUserMaterialParameters;
        if (m_base)
        {
            customTextures.push_back(m_base);
            backend::UniformType type = m_base->GetShaderString(UpdateUserMaterialParameters);
            UpdateUserMaterialParameters += "paras.baseColor = " + m_base->GetName() + ";\n";
        }
        PrepareForRasterGPUBase(customTextures, UpdateUserMaterialParameters);

        // update default parameters for materialInstance about uDefines
        if (m_bump)
            ;
        if (m_emission)
            ;
        if (m_base)
            ;

        m_isRasterGPUNeedUpdate = true;
        return true;
    }
    void GlossyCoatingMaterial::SetBase(const Texture* base)
    {
        m_base = base;
    }
} // namespace lightman
