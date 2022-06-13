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
        MatteTranslucentMaterial(const std::string& name, const Node* bump, const Node* emission, const Node* kd);
        ~MatteTranslucentMaterial();
        void SetKr(const Node* kr);
        void SetKt(const Node* kt);
        MaterialType getMaterialType() override { return Material::MaterialType::MATTETRANSLUCENT;};
        bool PrepareForRasterGPU() override;
    private:
        const Node* m_kr = nullptr;          // reflection color
        const Node* m_kt = nullptr;          // transmission color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTETRANSLUCENTMATERIAL_H
