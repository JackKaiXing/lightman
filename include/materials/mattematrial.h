#ifndef _LIGHTMAN_MATTEMATERIAL_H
#define _LIGHTMAN_MATTEMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MatteMaterial final : public Material
    {
    public:
        MatteMaterial(const std::string& name);
        MatteMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd);
        ~MatteMaterial();
        void SetKd(const Node* kd);
        MaterialType getMaterialType() override { return Material::MaterialType::MATTE;};
        bool PrepareForRasterGPU() override;
    private:
        const Node* m_kd = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTEMATERIAL_H
