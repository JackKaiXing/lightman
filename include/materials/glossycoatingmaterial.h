#ifndef _LIGHTMAN_GLOSSYCOATINGMATERIAL_H
#define _LIGHTMAN_GLOSSYCOATINGMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class GlossyCoatingMaterial final : public Material
    {
    public:
        GlossyCoatingMaterial(const std::string& name);
        GlossyCoatingMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~GlossyCoatingMaterial();
        void SetBase(const Texture* base);
        MaterialType getMaterialType() override { return Material::MaterialType::GLOSSYCOATING;};
        bool PrepareForRasterGPU() override;
    private:
        const Texture* m_base = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_GLOSSYCOATINGMATERIAL_H