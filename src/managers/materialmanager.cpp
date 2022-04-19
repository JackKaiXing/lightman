
#include "managers/materialmanager.h"
#include "materials/mattematrial.h"
#include <iostream>

namespace lightman
{
    MaterialManager::MaterialManager(/* args */)
    {
    }
    
    MaterialManager::~MaterialManager()
    {
    }

    Material* MaterialManager::CreateMaterial(Material::MaterialType type, const std::string& name)
    {
        std::unordered_map<std::string, Material*>::iterator iter = m_materials.find(name);
        if (iter != m_materials.end())
        {
            if (iter->second->getMaterialType() == type)
            {
                return iter->second;
            }
            // TODO, could create material with same name but different type, for editor later
            assert(0);
        }

        Material* result = nullptr;
        switch (type)
        {
        case Material::MaterialType::MATTE:
            result = new MatteMaterial(name);
            break;
        
        default:
            break;
        }
        m_materials.insert({name, result});
        return result;
    }

    MaterialInstance* MaterialManager::CreateMaterialInstance(Material* material, const std::string& name)
    {
        std::unordered_map<std::string, MaterialInstance*>::iterator iter = m_materialInstances.find(name);
        if (iter != m_materialInstances.end())
        {
            // TODO, could create material with same name but different type, for editor later
            assert(0);
        }
        
        MaterialInstance* result = nullptr;
        if (material)
        {
            result = material->createMaterialInstance(name);
            m_materialInstances.insert({name, result});
        }
        return result;
    }
}