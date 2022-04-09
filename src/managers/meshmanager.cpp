
#include "managers/meshmanager.h"
#include "geometry/trianglemesh.h"

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

    bool MeshManager::HasMesh(std::string name)
    {
        return m_meshes.find(name) != m_meshes.end();
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
        if (uvs.size()>0)
            mesh->InitUVs(uvs);

        m_meshes.insert({name, static_cast<Mesh*>(mesh)});
    }
}