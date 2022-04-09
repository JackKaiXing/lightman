

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

Transform& InstancedTriangleMesh::getTransform()
{
    return m_transform;
}

void InstancedTriangleMesh::setMesh(std::string name, Mesh* mesh)
{
    m_meshName = name;
    m_mesh = mesh;

    mesh->IncreaseRef();
}
    
} // namespace lightman