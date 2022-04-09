

#include "objects/instancedtrianglemesh.h"

namespace lightman
{
// ----------------------------------------------------------------------------
Transform& InstancedTriangleMesh::getTransform()
{
    return m_transform;
}

void InstancedTriangleMesh::setMeshName(std::string meshName)
{
    m_triMeshName = meshName;
}
    
} // namespace lightman