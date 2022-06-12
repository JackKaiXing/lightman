#ifndef _LIGHTMAN_GLOSSYMATERIAL_H
#define _LIGHTMAN_GLOSSYMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class GlossyMaterial final : public Material
    {
    public:
        GlossyMaterial(const std::string& name);
        GlossyMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~GlossyMaterial();
        void SetKd(const Texture* kd);
        MaterialType getMaterialType() override { return Material::MaterialType::GLOSSY;};
        bool PrepareForRasterGPU() override;
        
    private:
        const Texture* m_kd = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_GLOSSYMATERIAL_H
