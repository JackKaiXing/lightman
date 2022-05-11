

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
    if(m_mInstance)
        m_mInstance->ReleaseRef();
}

void InstancedTriangleMesh::SetTransform(const Matrix4X4& m)
{
    m_transform = m;
    m_needToUpdateTransform = true;
}

void InstancedTriangleMesh::setPVTransform(const Matrix4X4& pvmatrix)
{
    m_PVTransform = pvmatrix;
    m_needToUpdateTransform = true;
}

void InstancedTriangleMesh::SetMaterialInstance(MaterialInstance * mi)
{
    if (m_mInstance)
        m_mInstance->ReleaseRef();
    
    m_mInstance = mi;
    m_mInstance->IncreaseRef();
}

void InstancedTriangleMesh::SetMesh(std::string name, Mesh* mesh)
{
    m_meshName = name;
    m_mesh = dynamic_cast<TriangleMesh*>(mesh);

    mesh->IncreaseRef();
}

Mesh* InstancedTriangleMesh::GetMesh()
{
    return m_mesh;
}

std::string InstancedTriangleMesh::GetName()
{
    return m_meshName;
}

MaterialInstance* InstancedTriangleMesh::GetMaterialInstance()
{
    return m_mInstance;
}

uint32_t InstancedTriangleMesh::GetProgramIndexBySupportedVertexAttribute()
{
    return Material::GetProgramIndexBySupportedVertexAttribute(
        m_mesh->HasNormal(),m_mesh->HasUV(), false);
}

bool InstancedTriangleMesh::IsNeedToUpdateProgram()
{
    return m_needToUpdateProgram;
}

void InstancedTriangleMesh::UpdateProgram(HwProgram* program)
{
    m_program = program; // TODO REF
    m_needToUpdateProgram = true;
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
    {
        if(m_needToUpdateTransform)
        {
            // The OpenGL Matrix Buffer is column first, https://stackoverflow.com/questions/17717600/confusion-between-c-and-opengl-matrix-order-row-major-vs-column-major
            
            Matrix4X4 mMatrix = m_transform * m_mesh->GetTransform();
            
            Matrix4X4 pvmMatrix = m_PVTransform * mMatrix;
            pvmMatrix.Transpose();
            m_mInstance->SetParameter("PVMMatrix",pvmMatrix); // TODO Config
            
            Matrix4X4 TransposeInverseMMatrix = mMatrix.Inverse();
            // TransposeInverseMMatrix.Transpose().Transpose();
            // Normal Transformation, http://www.songho.ca/opengl/gl_normaltransform.html
            m_mInstance->SetParameter("InverseMMatrix",TransposeInverseMMatrix);
        }
        m_mInstance->SetParameter("uTestColor",0.5f); // TODO Config
        
        m_mInstance->Commit();
        m_mInstance->BindUniformBlockToProgram(m_program);
        m_mesh->Draw(m_program);
    }
#ifdef DEBUG
    else    
        std::cout << "InstancedMesh does not have a valid source mesh" << std::endl;
#endif
}
    
} // namespace lightman
