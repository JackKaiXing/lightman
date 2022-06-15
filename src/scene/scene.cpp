
#include "scene/scene.h"

namespace lightman
{
    Scene::Scene()
    {

    }
    Scene::~Scene()
    {
        if (GetRefCount() > 1)
        {
            ReleaseRef();
            return;
        }
        
        for (auto iter = m_iMeshes.begin(); iter != m_iMeshes.end(); iter++)
        {
            delete iter->second;
            iter->second = nullptr;
        }

        m_iMeshes.clear();
    }
    std::unordered_map<string, InstancedTriangleMesh*> Scene::GetInstanceMeshes()
    {
        return m_iMeshes;
    }
    InstancedTriangleMesh* Scene::GetMesh(const std::string& meshName)
    {
        auto iter = m_iMeshes.find(meshName);
        if( iter != m_iMeshes.end())
        {
            return iter->second;
        }
        return nullptr;
    }
    InstancedTriangleMesh* Scene::AddGetMesh(const std::string& meshName)
    {
        auto iter = m_iMeshes.find(meshName);
        if( iter != m_iMeshes.end())
        {
            return iter->second;
        }
        auto result = new InstancedTriangleMesh(meshName);
        m_iMeshes.insert({meshName,result});
        return result;
    }
}
