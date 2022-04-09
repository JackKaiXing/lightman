
#include "managers/meshmanager.h"

namespace lightman
{
    MeshManager * g_meshManager = nullptr;

    MeshManager* MeshManager::GetInstance()
    {
        if (g_meshManager==nullptr)
        {
           return new MeshManager();
        }
        return g_meshManager;
        
    }

    void MeshManager::DetoryInstance()
    {
        if (g_meshManager!=nullptr)
        {
           delete g_meshManager;
           g_meshManager = nullptr;
        }
    }

    MeshManager::MeshManager(/* args */)
    {
    }
    
    MeshManager::~MeshManager()
    {
    }
}