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
        ~MatteMaterial();
        MaterialType getMaterialType() override { return Material::MaterialType::MATTE;};
        static std::string CreateVertexShaderString(uint32_t index);
        static std::string CreateFragmentShaderString(uint32_t index);
    private:
        
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTEMATERIAL_H