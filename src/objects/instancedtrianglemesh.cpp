

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

void InstancedTriangleMesh::SetTransform(const Matrix4X4& m)
{
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

void InstancedTriangleMesh::Draw()
{
    if(m_mesh)  
        m_mesh->Draw();
#ifdef DEBUG
    else    
        std::cout << "InstancedMesh does not have a valid source mesh" << std::endl;
#endif
}
    
} // namespace lightman