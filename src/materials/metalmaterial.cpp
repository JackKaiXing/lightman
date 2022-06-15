
#include "materials/metalmaterial.h"
#include "materials/shader.h"

namespace lightman
{
    MetalMaterial::MetalMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    MetalMaterial::MetalMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd) 
        : Material(name,NULL,NULL)
    {
    }
    MetalMaterial::~MetalMaterial()
    {
        if(m_fresnel)
            delete m_fresnel;
    }
    bool MetalMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        // prepare uniform buffer with custom texture nodes
        std::vector<const Node*> customTextures;
        std::string UpdateUserMaterialParameters;
        if (m_fresnel)
        {
            customTextures.push_back(m_fresnel);
            backend::UniformType type = m_fresnel->GetShaderString(UpdateUserMaterialParameters);
            UpdateUserMaterialParameters += "paras.baseColor = " + m_fresnel->GetName() + ";\n";
            //UpdateUserMaterialParameters += "paras.metallic = 1.0f;\n";
        }
        PrepareForRasterGPUBase(customTextures, UpdateUserMaterialParameters);

        // update default parameters for materialInstance about uDefines
        if (m_bump)
            ;
        if (m_emission)
            ;
        if (m_fresnel)
            ;

        m_isRasterGPUNeedUpdate = true;
        return true;
    }
    void MetalMaterial::SetFresnel(const Node* kfresnel)
    {
        m_fresnel = kfresnel;
    }
} // namespace lightman
