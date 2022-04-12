

#include "objects/instancedtrianglemesh.h"

namespace lightman
{
// ----------------------------------------------------------------------------
InstancedTriangleMesh::InstancedTriangleMesh()
{

}
InstancedTriangleMesh::~InstancedTriangleMesh()
{
    if(m_mesh)
        m_mesh->ReleaseRef();
}

Transform& InstancedTriangleMesh::GetTransform()
{
    return m_transform;
}

void InstancedTriangleMesh::SetMesh(std::string name, Mesh* mesh)
{
    m_meshName = name;
    m_mesh = mesh;

    mesh->IncreaseRef();
}

void InstancedTriangleMesh::PrepareForRasterGPU()
{
    if(m_mesh)  
        m_mesh->PrepareForRasterGPU();
#ifdef DEBUG
    else    
        std::cout << "InstancedMesh does not have a valid source mesh" << std::endl;
#endif
}
    
} // namespace lightman