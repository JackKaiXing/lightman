
#include "managers/materialmanager.h"
#include "materials/materialtypeheaders.h"
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
        for (auto iter = m_materialInstances.begin(); iter != m_materialInstances.end(); iter++)
        {
            if (iter->second->GetRefCount() > 1)
                std::cout << "Warning: Delete MatrialManager While MInstance Being Referenced!" << std::endl;
            
            delete iter->second;
        }
        m_materialInstances.clear();
        
        for (auto iter = m_materials.begin(); iter != m_materials.end(); iter++)
        {
            if (iter->second->GetRefCount() > 2)
                std::cout << "Warning: Delete MatrialManager While Material Being Referenced!" << std::endl;
            
            delete iter->second;
        }
        m_materials.clear();
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

        Material* result = nullptr;
        switch (type)
        {
        case Material::MaterialType::MATTE:
            result = new MatteMaterial(name);
            break;
        case Material::MaterialType::GLOSSY:
            result = new GlossyMaterial(name);
            break;
        case Material::MaterialType::METAL:
            result = new MetalMaterial(name);
            break;
        case Material::MaterialType::MATTETRANSLUCENT:
            result = new MatteTranslucentMaterial(name);
            break;
        case Material::MaterialType::GLASS:
        case Material::MaterialType::ARCHGLASS:
            result = new GlassMaterial(name);
            break;
        case Material::MaterialType::VELVET:
            result = new VelvetMaterial(name);
            break;
        case Material::MaterialType::GLOSSYCOATING:
            result = new GlossyCoatingMaterial(name);
            break;
        case Material::MaterialType::MIX:
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
