
#include "managers/meshmanager.h"
#include "geometry/trianglemesh.h"

namespace lightman
{
    MeshManager * g_meshManager = nullptr;

    MeshManager* MeshManager::GetInstance()
    {
        if (g_meshManager==nullptr)
        {
            g_meshManager = new MeshManager();
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
        for (auto iter = m_meshes.begin(); iter != m_meshes.end(); iter++)
        {
            if (iter->second->GetRefCount() > 1)
                std::cout << "Warning: Delete MeshManager While Mesh Being Referenced!" << std::endl;
            
            delete iter->second;
        }
        m_meshes.clear();
    }

    bool MeshManager::HasMesh(std::string name)
    {
        return m_meshes.find(name) != m_meshes.end();
    }

    Mesh* MeshManager::GetMesh(std::string name)
    {
        return m_meshes.find(name)->second;
    }

    void MeshManager::CreateTriMesh(std::string name,
            std::vector<unsigned int>& triIndexs, std::vector<float>& points,
            std::vector<float>& normals, std::vector<float>& uvs)
    {
        if ( m_meshes.find(name) != m_meshes.end() )
        {
            assert(0);
        }

        TriangleMesh * mesh = new TriangleMesh(triIndexs,points);
        if (normals.size()>0)
            mesh->InitNormals(normals);
        else
            mesh->GenerateVertexNormals();
        if (uvs.size()>0)
            mesh->InitUVs(uvs);

        m_meshes.insert({name, static_cast<Mesh*>(mesh)});
    }
}
