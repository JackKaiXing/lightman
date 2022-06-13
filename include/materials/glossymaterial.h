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
        GlossyMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd);
        ~GlossyMaterial();
        void SetKd(const Node* kd);
        MaterialType getMaterialType() override { return Material::MaterialType::GLOSSY;};
        bool PrepareForRasterGPU() override;
        
    private:
        const Node* m_kd = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_GLOSSYMATERIAL_H
