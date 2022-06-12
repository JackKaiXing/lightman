
#include "materials/glassmaterial.h"
#include "materials/shader.h"

namespace lightman
{
    GlassMaterial::GlassMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    GlassMaterial::GlassMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd) 
        : Material(name,NULL,NULL)
    {
    }
    GlassMaterial::~GlassMaterial()
    {
    }
    bool GlassMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        // prepare uniform buffer with custom texture nodes
        std::vector<const Texture*> customTextures;
        std::string UpdateUserMaterialParameters;
        if (m_kr)
        {
            customTextures.push_back(m_kr);
            backend::UniformType type = m_kr->GetShaderString(UpdateUserMaterialParameters);
            UpdateUserMaterialParameters += "paras.baseColor = " + m_kr->GetName() + ";\n";
        }
        PrepareForRasterGPUBase(customTextures, UpdateUserMaterialParameters);

        // update default parameters for materialInstance about uDefines
        if (m_bump)
            ;
        if (m_emission)
            ;
        if (m_kr)
            ;

        m_isRasterGPUNeedUpdate = true;
        return true;
    }
    void GlassMaterial::SetKr(const Texture* kr)
    {
        m_kr = kr;
    }
    void GlassMaterial::SetKt(const Texture* kt)
    {
        m_kt = kt;
    }
} // namespace lightman