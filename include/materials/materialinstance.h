#ifndef _LIGHTMAN_MATERIALINSTANCE_H
#define _LIGHTMAN_MATERIALINSTANCE_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class Material;
    class MaterialInstance
    {
    friend class Material;
    protected:
        MaterialInstance(Material * material, const std::string& name);
        ~MaterialInstance();
    private:
        std::string m_name;
        Material * m_material = nullptr;
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALINSTANCE_H