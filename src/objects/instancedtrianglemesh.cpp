

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
    
    // TransposeInverseMMatrix.Transpose().Transpose();
    // Normal Transformation, http://www.songho.ca/opengl/gl_normaltransform.html
    m_transposeInverseMMatrix = m_transform * m_mesh->GetTransform();
    m_transposeInverseMMatrix.Inverse();
    m_transposeInverseMMatrix.Transpose();
}

void InstancedTriangleMesh::setPVTransform(const Matrix4X4& pvmatrix)
{
    m_PVMTransform = pvmatrix * m_transform * m_mesh->GetTransform();
}

void InstancedTriangleMesh::SetMaterial(Material* mat)
{
    if (m_mInstance)
    {
        m_mInstance->ReleaseRef();
        m_mInstance = nullptr;
    }

    if (m_material)
        m_material->ReleaseRef();
    
    m_material = mat;
    m_material->IncreaseRef();
}

void InstancedTriangleMesh::SetMaterialInstance(MaterialInstance * mi)
{
    if (m_material)
    {
        m_material->ReleaseRef();
        m_material = nullptr;
    }
        
    if (m_mInstance)
        m_mInstance->ReleaseRef();
    
    m_mInstance = mi;
    if(m_mInstance)
        m_mInstance->IncreaseRef();
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

bool InstancedTriangleMesh::hasMaterial()
{
    return m_mInstance || m_material;
}

uint32_t InstancedTriangleMesh::GetProgramIndexBySupportedVertexAttribute()
{
    return Material::GetProgramIndexBySupportedVertexAttribute(
        m_mesh->HasNormal(),m_mesh->HasUV(), false);
}

void InstancedTriangleMesh::PrepareForRasterGPU()
{
    if(m_mesh)
        m_mesh->PrepareForRasterGPU();
#ifdef DEBUG
    else    
        std::cout << "InstancedMesh does not have a valid source mesh" << std::endl;
#endif

    if (m_mInstance)
        m_mInstance->GetMaterial()->PrepareForRasterGPU();

    if (m_material)
        m_material->PrepareForRasterGPU();
}

void InstancedTriangleMesh::Draw()
{
    if(m_mesh)  
    {
        MaterialInstance * mi = nullptr;
        if (m_mInstance)
            mi = m_mInstance;
        else
            mi = m_material->GetDefaultMaterialInstance();
        
        /*
            update uniforms : since material/materialinstance is shared between meshes,
            there is great possibily that the uniform buffer should be updated.
        */
        {
            // The OpenGL Matrix Buffer is column first,
            // https://stackoverflow.com/questions/17717600/confusion-between-c-and-opengl-matrix-order-row-major-vs-column-major
            Matrix4X4 pvmMatrix = m_PVMTransform;
            pvmMatrix.Transpose();
            Matrix4X4 transposeInverseMMatrix = m_transposeInverseMMatrix;
            transposeInverseMMatrix.Transpose();
            
            mi->SetParameter("PVMMatrix",pvmMatrix); // TODO Config
            mi->SetParameter("InverseMMatrix",transposeInverseMMatrix);
            mi->Commit();
        }
        m_mesh->Draw(mi->GetMaterial()->GetProgram());
    }
#ifdef DEBUG
    else    
        std::cout << "InstancedMesh does not have a valid source mesh" << std::endl;
#endif
}
    
} // namespace lightman
