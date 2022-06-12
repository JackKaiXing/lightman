#ifndef _LIGHTMAN_MATTETRANSLUCENTMATERIAL_H
#define _LIGHTMAN_MATTETRANSLUCENTMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MatteTranslucentMaterial final : public Material
    {
    public:
        MatteTranslucentMaterial(const std::string& name);
        MatteTranslucentMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~MatteTranslucentMaterial();
        void SetKr(const Texture* kr);
        void SetKt(const Texture* kt);
        MaterialType getMaterialType() override { return Material::MaterialType::MATTETRANSLUCENT;};
        bool PrepareForRasterGPU() override;
    private:
        const Texture* m_kr = nullptr;          // reflection color
        const Texture* m_kt = nullptr;          // transmission color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTETRANSLUCENTMATERIAL_H
