

#include "objects/instancedtrianglemesh.h"

namespace lightman
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
    
} // namespace lightman