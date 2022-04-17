
#include "scene/scene.h"

namespace lightman
{
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
    InstancedTriangleMesh* Scene::AddMesh(const std::string& meshName)
    {
        auto iter = m_iMeshes.find(meshName);
        if( iter != m_iMeshes.end())
        {
            return iter->second;
        }
        auto result = new InstancedTriangleMesh();
        m_iMeshes.insert({meshName,result});
        return result;
    }
}