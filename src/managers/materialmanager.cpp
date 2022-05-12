
#include "managers/materialmanager.h"
#include "materials/mattematrial.h"
#include <iostream>

namespace lightman
{
    MaterialManager * g_materialManager = nullptr;

    MaterialManager* MaterialManager::GetInstance()
    {
        if (g_materialManager==nullptr)
        {
            g_materialManager = new MaterialManager();
        }
        return g_materialManager;
        
    }

    void MaterialManager::DetoryInstance()
    {
        if (g_materialManager!=nullptr)
        {
           delete g_materialManager;
           g_materialManager = nullptr;
        }
    }

    MaterialManager::MaterialManager(/* args */)
    {
    }
    
    MaterialManager::~MaterialManager()
    {
    }

    Material* MaterialManager::GetMaterial(const std::string& name)
    {
        Material* result = nullptr;
        std::unordered_map<std::string, Material*>::iterator iter = m_materials.find(name);
        if (iter != m_materials.end())
        {
            result = iter->second;
        }
        return result;
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

        // TODO after import material params and texture, do seperate materials implementations
        Material* result = nullptr;
        switch (type)
        {
        case Material::MaterialType::MATTE:
        case Material::MaterialType::GLOSSY2:
        case Material::MaterialType::MATTETRANSLUCENT:
        case Material::MaterialType::METAL2:
        case Material::MaterialType::ARCHGLASS:
        case Material::MaterialType::VELVET:
        case Material::MaterialType::MIX:
        case Material::MaterialType::GLOSSYCOATING:
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
            return iter->second;
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
