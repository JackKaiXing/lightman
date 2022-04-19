#ifndef _LIGHTMAN_MATTEMATERIAL_H
#define _LIGHTMAN_MATTEMATERIAL_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MatteMaterial final : public Material
    {
    public:
        MatteMaterial();
        ~MatteMaterial();
        MaterialType getMaterialType() { return Material::MaterialType::MATTE;};
    protected:
        
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATTEMATERIAL_H