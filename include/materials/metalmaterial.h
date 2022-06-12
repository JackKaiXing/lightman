#ifndef _LIGHTMAN_METALMATERIAL_H
#define _LIGHTMAN_METALMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MetalMaterial final : public Material
    {
    public:
        MetalMaterial(const std::string& name);
        MetalMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~MetalMaterial();
        void SetFresnel(const Texture* kfresnel);
        MaterialType getMaterialType() override { return Material::MaterialType::METAL;};
        bool PrepareForRasterGPU() override;
        
    private:
        const Texture* m_fresnel = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_METALMATERIAL_H