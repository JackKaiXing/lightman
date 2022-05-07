
#include <assert.h>

#include "geometry/trianglemesh.h"
#include "math/vector.h"
#include "engine/engine.h"

namespace lightman
{
namespace geometry
{
// ----------------------------------------------------------------------------
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
            uint32_t size = 0;
            void* data = nullptr;
            switch (i)
            {
            case backend::VertexAttribute::POSITION:
                size = m_points.size() * sizeof(float);
                data = (void*)m_points.data();
                break;
            case backend::VertexAttribute::TANGENTS:
                size = m_normals.size() * sizeof(float);
                data = (void*)m_normals.data();
                break;
            case backend::VertexAttribute::UV0:
                size = m_uvs.size() * sizeof(float);
                data = (void*)m_uvs.data();
                break;
            default:
                break;
            }
            HwBufferObject * bo = Engine::GetInstance()->GetDriver()->createBufferObject(size, 
                backend::BufferObjectBinding::VERTEX, backend::BufferUsage::STATIC);
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
    
    // TODO UPDATE NORMAL Vertex Attribute Buffer
    // https://pbr-book.org/3ed-2018/Geometry_and_Transformations/Applying_Transformations#Transform::SwapsHandedness
    if (m_normals.size() > 0 && m_appliedTransform.SwapsHandedness())
    {
        Vector4 result;
        for (size_t i = 0; i < m_normals.size(); i++)
        {
            m_normals.at(i) *= -1.0;
        }
    }
}
} // namespace geometry
} // namespace lightman
