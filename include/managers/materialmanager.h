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
    private:
        std::unordered_map<std::string, Material*> m_mats;
    };
    
    MaterialManager::MaterialManager(/* args */)
    {
    }
    
    MaterialManager::~MaterialManager()
    {
    }
} // namespace lightman

#endif  //_LIGHTMAN_MATERIALMANAGER_H