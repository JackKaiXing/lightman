
#include <assert.h>

#include "geometry/trianglemesh.h"
#include "math/vector.h"
#include "engine/engine.h"

namespace lightman
{
namespace geometry
{
// ----------------------------------------------------------------------------
 TriangleMesh::~TriangleMesh()
{
    m_triIndexs.clear();
    m_points.clear();
    m_normals.clear();
    m_uvs.clear();

    backend::Driver* driver = Engine::GetInstance()->GetDriver();
    if (m_renderPrimitive)
        driver->DestroyRenderPrimitive(m_renderPrimitive);
    
    if (m_indexBuffer)
        driver->DestroyIndexBuffer(m_indexBuffer);
    
    if (m_vertexBuffer)
        driver->DestroyVertexBuffer(m_vertexBuffer);
    
    if (m_posBuffetObject)
        driver->DestroyBufferObject(m_posBuffetObject);
    
    if (m_normalBuffetObject)
        driver->DestroyBufferObject(m_normalBuffetObject);

    if (m_uvBuffetObject)
        driver->DestroyBufferObject(m_uvBuffetObject);
    
}
TriangleMesh::TriangleMesh(std::vector<unsigned int>& triIndexs, std::vector<float>& points)
{
    assert(points.size() > 9 && triIndexs.size() > 3);

    // fast copy, https://en.cppreference.com/w/cpp/utility/move
    // https://stackoverflow.com/questions/2740020/c-stl-array-vs-vector-raw-element-accessing-performance
    // vector access or pointer access is of same performance, so we use vector here
    m_points = std::move(points);
    m_triIndexs = std::move(triIndexs);

    points.clear();
    triIndexs.clear(); 

    SetAttribute(backend::VertexAttribute::POSITION, backend::ElementType::FLOAT3, 
        0, 0, 0); 
}
void TriangleMesh::SetAttribute(backend::VertexAttribute attributeType, backend::ElementType elementType, uint8_t flags, uint8_t stride, uint32_t offset)
{
    m_declaredAttribute += 1 << attributeType;
    m_attributeArray[attributeType].offset = offset;
    m_attributeArray[attributeType].stride = stride;
    m_attributeArray[attributeType].type = elementType;
    m_attributeArray[attributeType].flags |= flags;
    m_attributeArray[attributeType].buffer = attributeType;
}
void TriangleMesh::GenerateVertexNormals()
{
    std::vector<uint32_t> vCount(m_points.size()/3, 0);
    m_normals.clear();
    m_normals.resize(m_points.size());
    std::fill(m_normals.begin(), m_normals.end(), 0.0);

    uint32_t v0, v1, v2;
    Vector3 p0, p1, p2;
    Vector3 p01, p21;
    Vector3 n;
    for (int i = 0, j = 0; i < m_triIndexs.size(); i = i + 3)
    {
        v0 = m_triIndexs.at(i + 0);
        v1 = m_triIndexs.at(i + 1);
        v2 = m_triIndexs.at(i + 2);
        
        p0.v[0] = m_points.at(3 * v0 + 0); p0.v[1] = m_points.at(3 * v0 + 1); p0.v[2] = m_points.at(3 * v0 + 2);
        p1.v[0] = m_points.at(3 * v1 + 0); p1.v[1] = m_points.at(3 * v1 + 1); p1.v[2] = m_points.at(3 * v1 + 2);
        p2.v[0] = m_points.at(3 * v2 + 0); p2.v[1] = m_points.at(3 * v2 + 1); p2.v[2] = m_points.at(3 * v2 + 2);

        p01 = p0 - p1;
        p21 = p2 - p1;

        n = Vector3::Cross(p21, p01);

        vCount.at(v0) += 1;
        vCount.at(v1) += 1;
        vCount.at(v2) += 1;

        j = 0; m_normals.at(v0 * 3 + j) += n.v[j]; j++; m_normals.at(v0 * 3 + j) += n.v[j]; j++; m_normals.at(v0 * 3 + j) += n.v[j];
        j = 0; m_normals.at(v1 * 3 + j) += n.v[j]; j++; m_normals.at(v1 * 3 + j) += n.v[j]; j++; m_normals.at(v1 * 3 + j) += n.v[j];
        j = 0; m_normals.at(v2 * 3 + j) += n.v[j]; j++; m_normals.at(v2 * 3 + j) += n.v[j]; j++; m_normals.at(v2 * 3 + j) += n.v[j];
    }

    for (int i = 0; i < m_normals.size(); i = i + 3)
    {
        int j = i / 3;

        uint32_t count = vCount.at(j);

        m_normals.at(i + 0) /= count;
        m_normals.at(i + 1) /= count;
        m_normals.at(i + 2) /= count;
    }

    SetAttribute(backend::VertexAttribute::TANGENTS, backend::ElementType::FLOAT3, 
        backend::Attribute::FLAG_NORMALIZED, 0, 0); 
    
}
void TriangleMesh::InitNormals(std::vector<float>& normals)
{
    assert(normals.size() == m_points.size());
    m_normals = std::move(normals);
    normals.clear();

    // TODO Quaterion FLOAT4
    SetAttribute(backend::VertexAttribute::TANGENTS, backend::ElementType::FLOAT3, 
        backend::Attribute::FLAG_NORMALIZED, 0, 0); 
}
void TriangleMesh::InitUVs(std::vector<float>& uvs)
{
    assert(uvs.size()/2 == m_points.size()/3);
    m_uvs = std::move(uvs);
    uvs.clear();

    SetAttribute(backend::VertexAttribute::UV0, backend::ElementType::FLOAT2, 
        0, 0, 0); 
}
void TriangleMesh::PrepareForRasterGPU()
{
    if(m_isRasterGPUInitialized)
        return;
    
    // vao
    m_renderPrimitive = Engine::GetInstance()->GetDriver()->createRenderPrimitive();
    m_renderPrimitive->offset = 0;
    m_renderPrimitive->minIndex = 0;
    m_renderPrimitive->maxIndex = m_triIndexs.size();
    m_renderPrimitive->count = m_triIndexs.size();

    // vertex buffer
    uint8_t attributeCount = 0;
    for (size_t i = 0; i < backend::MAX_VERTEX_ATTRIBUTE_COUNT; i++)
    {
        if(m_declaredAttribute & (1 << i))
        {
            attributeCount++;
        }
    }
    m_vertexBuffer = Engine::GetInstance()->GetDriver()->createVertexBuffer(
        0,attributeCount,m_points.size()/3,m_attributeArray);
    for (size_t i = 0; i < backend::MAX_VERTEX_ATTRIBUTE_COUNT; i++)
    {
        if(m_declaredAttribute & (1 << i))
        {
            HwBufferObject * bo = nullptr;
            uint32_t size = sizeof(float);
            void* data = nullptr;
            switch (i)
            {
            case backend::VertexAttribute::POSITION:
                {
                    size *= m_points.size();
                    data = (void*)m_points.data();
                    m_posBuffetObject = Engine::GetInstance()->GetDriver()->createBufferObject(size, backend::BufferObjectBinding::VERTEX, backend::BufferUsage::STATIC);
                    bo = m_posBuffetObject;
                }
                break;
            case backend::VertexAttribute::TANGENTS:
                {
                    size *= m_normals.size();
                    data = (void*)m_normals.data();
                    m_normalBuffetObject = Engine::GetInstance()->GetDriver()->createBufferObject(size, backend::BufferObjectBinding::VERTEX, backend::BufferUsage::STATIC);
                    bo = m_normalBuffetObject;
                }
                break;
            case backend::VertexAttribute::UV0:
                {
                    size *= m_uvs.size();
                    data = (void*)m_uvs.data();
                    m_uvBuffetObject = Engine::GetInstance()->GetDriver()->createBufferObject(size, backend::BufferObjectBinding::VERTEX, backend::BufferUsage::STATIC);
                    bo = m_uvBuffetObject;
                }
                break;
            default:
                break;
            }
            
            Engine::GetInstance()->GetDriver()->updateBufferObject(bo, data, size, 0);
            // NOTE: i is in the order of VertexAttribute
            Engine::GetInstance()->GetDriver()->setVertexBufferObject(m_vertexBuffer,i,bo);
        }
    }

    // index buffer
    m_indexBuffer = Engine::GetInstance()->GetDriver()->createIndexBuffer(backend::ElementType::UINT, 
        m_triIndexs.size(), backend::BufferUsage::STATIC);
    Engine::GetInstance()->GetDriver()->updateIndexBuffer(m_indexBuffer,m_triIndexs.data(), m_triIndexs.size() * sizeof(uint32_t), 0);
    
    // set renderprimitive
    Engine::GetInstance()->GetDriver()->setRenderPrimitiveBuffer(m_renderPrimitive, m_vertexBuffer, m_indexBuffer);

    m_isRasterGPUInitialized = true;
}
void TriangleMesh::Draw(backend::HwProgram * program)
{
    if(!m_isRasterGPUInitialized)
        return;
    Engine::GetInstance()->GetDriver()->draw(program, m_renderPrimitive);
}
Matrix4X4 TriangleMesh::GetTransform()
{
    return m_transform;
}
void TriangleMesh::SetAppliedTransform(const Matrix4X4& mat)
{
    m_appliedTransform = mat;
    
    // https://pbr-book.org/3ed-2018/Geometry_and_Transformations/Applying_Transformations#Transform::SwapsHandedness
    if (m_normals.size() > 0 && m_appliedTransform.SwapsHandedness())
    {
        Vector4 result;
        for (size_t i = 0; i < m_normals.size(); i++)
        {
            m_normals.at(i) *= -1.0;
        }

        // UPDATE NORMAL Vertex Attribute Buffer
        if (m_isRasterGPUInitialized)
        {
            uint32_t size = m_normals.size() * sizeof(float);
            void* data = (void*)m_normals.data();

            Engine::GetInstance()->GetDriver()->updateBufferObject(m_normalBuffetObject, data, size, 0);
        }
    }
}
} // namespace geometry
} // namespace lightman
