#ifndef _LIGHTMAN_GLASSMATERIAL_H
#define _LIGHTMAN_GLASSMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class GlassMaterial final : public Material
    {
    public:
        GlassMaterial(const std::string& name);
        GlassMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd);
        ~GlassMaterial();
        void SetKr(const Node* kr);
        void SetKt(const Node* kt);
        MaterialType getMaterialType() override { return Material::MaterialType::GLASS;};
        bool PrepareForRasterGPU() override;
    private:
        const Node* m_kr = nullptr;          // reflection color
        const Node* m_kt = nullptr;          // transmission color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_GLASSMATERIAL_H
