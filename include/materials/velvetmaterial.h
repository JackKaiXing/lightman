#ifndef _LIGHTMAN_VELVETMATERIAL_H
#define _LIGHTMAN_VELVETMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class VelvetMaterial final : public Material
    {
    public:
        VelvetMaterial(const std::string& name);
        VelvetMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~VelvetMaterial();
        void SetKd(const Texture* kd);
        MaterialType getMaterialType() override { return Material::MaterialType::VELVET;};
        bool PrepareForRasterGPU() override;
    private:
        const Texture* m_kd = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_VELVETMATERIAL_H
