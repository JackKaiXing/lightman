#ifndef _LIGHTMAN_MATERIALMANAGER_H
#define _LIGHTMAN_MATERIALMANAGER_H

#include <unordered_map>
#include <string>

#include "materials/material.h"

namespace lightman
{
    class Material;
    class MaterialManager
    {
    public:
        MaterialManager(/* args */);
        ~MaterialManager();
        Material* CreateMaterial(Material::MaterialType type, const std::string& name);
        MaterialInstance* CreateMaterialInstance(Material* material, const std::string& name);

    private:
        std::unordered_map<std::string, Material*> m_materials;
        std::unordered_map<std::string, MaterialInstance*> m_materialInstances;
    };
} // namespace lightman

#endif  //_LIGHTMAN_MATERIALMANAGER_H