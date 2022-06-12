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
        GlassMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~GlassMaterial();
        void SetKr(const Texture* kr);
        void SetKt(const Texture* kt);
        MaterialType getMaterialType() override { return Material::MaterialType::GLASS;};
        bool PrepareForRasterGPU() override;
    private:
        const Texture* m_kr = nullptr;          // reflection color
        const Texture* m_kt = nullptr;          // transmission color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_GLASSMATERIAL_H
