
#include "materials/glossycoatingmaterial.h"
#include "materials/shader.h"

namespace lightman
{
    GlossyCoatingMaterial::GlossyCoatingMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    GlossyCoatingMaterial::GlossyCoatingMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd) 
        : Material(name, bump, emission)
    {
    }
    GlossyCoatingMaterial::~GlossyCoatingMaterial()
    {
        if(m_base)
            delete m_base;
    }
    bool GlossyCoatingMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        // prepare uniform buffer with custom texture nodes
        std::vector<const Node*> customTextures;
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
    void GlossyCoatingMaterial::SetBase(const Node* base)
    {
        m_base = base;
    }
} // namespace lightman
