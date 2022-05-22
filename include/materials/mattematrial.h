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
        MatteMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd);
        ~MatteMaterial();
        MaterialType getMaterialType() override { return Material::MaterialType::MATTE;};
        bool PrepareForRasterGPU() override;
        std::string CreateVertexShaderString();
        std::string CreateFragmentShaderString();
        static backend::UniformBlockInfo GetUniformBufferInfo();
    private:
        const Texture* m_kd = nullptr;          // diffuse color
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTEMATERIAL_H
