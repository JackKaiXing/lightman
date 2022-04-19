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
        MaterialType getMaterialType() { return Material::MaterialType::MATTE;};
    private:
        
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTEMATERIAL_H