

#include "objects/instancedtrianglemesh.h"
#include "materials/shader.h"

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
        RELEASEORDELETE(m_mesh);
    if(m_mInstance)
        RELEASEORDELETE(m_mInstance);
    if(m_material)
        RELEASEORDELETE(m_material);
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

void InstancedTriangleMesh::setPVTransform(const Matrix4X4& pvmatrix, const Vector3& cameraPos)
{
    m_PVMTransform = pvmatrix * m_transform * m_mesh->GetTransform();
    m_cameraPos = cameraPos;
}

void InstancedTriangleMesh::SetMaterial(Material* mat)
{
    if (m_mInstance)
        RELEASEORDELETE(m_mInstance);

    if (m_material)
        RELEASEORDELETE(m_material);
    
    m_material = mat;
    if (m_material)
         m_material->IncreaseRef();
    m_mInstance = nullptr;
}

void InstancedTriangleMesh::SetMaterialInstance(MaterialInstance * mi)
{
    if (m_material)
        RELEASEORDELETE(m_material);
        
    if (m_mInstance)
        RELEASEORDELETE(m_mInstance);
    
    m_mInstance = mi;
    if(m_mInstance)
        m_mInstance->IncreaseRef();
    m_material = nullptr;
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
            
            mi->SetParameter(ShaderString::SharedUniformName_PVMMatrix.c_str(),pvmMatrix);
            mi->SetParameter(ShaderString::SharedUniformName_InverseMMatrix.c_str(),transposeInverseMMatrix);
            mi->SetParameter(ShaderString::SharedUniformName_HasNormal.c_str(), m_mesh->HasNormal()? 1 : 0);
            mi->SetParameter(ShaderString::SharedUniformName_HasUV0.c_str(),m_mesh->HasUV()? 1 : 0);
            mi->SetParameter(ShaderString::SharedUniformName_HasUV1.c_str(),false? 1 : 0);
            {
                // TODO remove into a unique block for all raster objects
                mi->SetParameter(ShaderString::SharedUniformName_CameraPos.c_str(), m_cameraPos);

                Vector4 ambientColor = Vector4(0.1, 0.1, 0.1, 1.0);
                mi->SetParameter("world_light_ambient_color", ambientColor);

                Vector4 light0Direction = Vector4(-0.352546f, 0.170931f, -0.920051f, 1.0f);
                Vector4 light0Spec = Vector4(0.266761f / math::L_PI, 0.266761f / math::L_PI, 0.266761f / math::L_PI, 1.0f);
                Vector4 light0DiffuseWrap = Vector4(0.33103f, 0.33103f, 0.33103f, 0.526620f);
                mi->SetParameter("world_light_direction", light0Direction, 0);
                mi->SetParameter("world_light_specular_color", light0Spec, 0);
                mi->SetParameter("world_light_diffuse_color_wrap", light0DiffuseWrap, 0);

                Vector4 light1Direction = Vector4(-0.408163f, 0.346939f, 0.844415f, 1.0f);
                Vector4 light1Spec = Vector4(0.599030f / math::L_PI, 0.599030f / math::L_PI, 0.599030f / math::L_PI, 1.0f);
                Vector4 light1DiffuseWrap = Vector4(0.521083f, 0.521083f, 0.521083f, 0.000000f);
                mi->SetParameter("world_light_direction", light1Direction, 1);
                mi->SetParameter("world_light_specular_color", light1Spec, 1);
                mi->SetParameter("world_light_diffuse_color_wrap", light1DiffuseWrap, 1);

                Vector4 light2Direction = Vector4(0.521739f, 0.826087f, 0.212999f, 1.0f);
                Vector4 light2Spec = Vector4(0.125981f / math::L_PI, 0.125981f / math::L_PI, 0.125981f / math::L_PI, 1.0f);
                Vector4 light2DiffuseWrap = Vector4(0.38403f, 0.38403f, 0.38403f, 0.478261f);
                mi->SetParameter("world_light_direction", light2Direction, 2);
                mi->SetParameter("world_light_specular_color", light2Spec, 2);
                mi->SetParameter("world_light_diffuse_color_wrap", light2DiffuseWrap, 2);

                Vector4 light3Direction = Vector4(0.624519f, -0.562067f, -0.542269f, 1.0f);
                Vector4 light3Spec = Vector4(0.106535f / math::L_PI, 0.106535f / math::L_PI, 0.106535f / math::L_PI, 1.0f);
                Vector4 light3DiffuseWrap = Vector4(0.82080f, 0.82080f, 0.82080f, 0.200000f);
                mi->SetParameter("world_light_direction", light3Direction, 3);
                mi->SetParameter("world_light_specular_color", light3Spec, 3);
                mi->SetParameter("world_light_diffuse_color_wrap", light3DiffuseWrap, 3);
            }
            
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
