
#include "materials/material.h"

namespace lightman
{
    Material::Material(const std::string& name)
    {
        m_name = name;
    }
    MaterialInstance* Material::createMaterialInstance(const std::string& name)
    {
        MaterialInstance* result = new MaterialInstance(this, name);
        return result;
    }
}