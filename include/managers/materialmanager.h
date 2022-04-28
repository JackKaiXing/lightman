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
        static MaterialManager* GetInstance();
        static void DetoryInstance();
        Material* GetMaterial(const std::string& name);
        Material* CreateMaterial(Material::MaterialType type, const std::string& name);
        MaterialInstance* CreateMaterialInstance(Material* material, const std::string& name);
    protected:
        MaterialManager(/* args */);
        ~MaterialManager();

    private:
        std::unordered_map<std::string, Material*> m_materials;
        std::unordered_map<std::string, MaterialInstance*> m_materialInstances;
    };
} // namespace lightman

#endif  //_LIGHTMAN_MATERIALMANAGER_H