
#include "materials/material.h"

namespace lightman
{
    Material::Material(const std::string& name)
    {
        m_name = name;
    }
    MaterialInstance* Material::createMaterialInstance(const std::string& name)
    {
        IncreaseRef();
        
        MaterialInstance* result = new MaterialInstance(this, name);
        return result;
    }
}