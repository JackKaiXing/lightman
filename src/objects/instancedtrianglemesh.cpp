

#include "objects/instancedtrianglemesh.h"

namespace lightman
{
namespace objects
{
// ----------------------------------------------------------------------------
Transform& InstancedTriangleMesh::getTransform()
{
    return m_transform;
}

void InstancedTriangleMesh::setMesh(TriangleMesh * _mesh)
{
    m_triMesh = _mesh;
}

void InstancedTriangleMesh::preRender()
{
    m_triMesh->preRender();
}
} // namespace objects
    
} // namespace lightman