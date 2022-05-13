

#include "objects/instancedtrianglemesh.h"

namespace lightman
{
// ----------------------------------------------------------------------------
InstancedTriangleMesh::InstancedTriangleMesh(const std::string& name)
{
    m_name = name;
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
    Material::MaterialType src = Material::MaterialType::MAX_MATERIALTYPE_COUNT;
    if (m_mInstance)
    {
        m_mInstance->ReleaseRef();
        src = m_mInstance->GetMaterial()->getMaterialType();
    }
    
    m_mInstance = mi;
    Material::MaterialType dst = Material::MaterialType::MAX_MATERIALTYPE_COUNT;
    if(m_mInstance)
    {
        m_mInstance->IncreaseRef();
        dst = m_mInstance->GetMaterial()->getMaterialType();
    }

    if(src != dst)
    {
        m_needToUpdateProgram = true;
    }
    
}

void InstancedTriangleMesh::SetMesh(std::string name, Mesh* mesh)
{
    m_masterName = name;
    m_mesh = dynamic_cast<TriangleMesh*>(mesh);

    mesh->IncreaseRef();
}

Mesh* InstancedTriangleMesh::GetMesh()
{
    return m_mesh;
}

std::string InstancedTriangleMesh::GetName()
{
    return m_name;
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
    m_mInstance->BindUniformBlockToProgram(m_program);
    m_needToUpdateProgram = false;
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
        m_mInstance->SetParameter("uTestColor",1.0f); // TODO Config
        m_mInstance->Commit();
        m_mesh->Draw(m_program);
    }
#ifdef DEBUG
    else    
        std::cout << "InstancedMesh does not have a valid source mesh" << std::endl;
#endif
}
    
} // namespace lightman
