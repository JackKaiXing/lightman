
#include <assert.h>

#include "geometry/trianglemesh.h"
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

    // TODO BUFFERINDEX
    SetAttribute(0, backend::VertexAttribute::POSITION, backend::ElementType::FLOAT3, 
        0, 0, 0); 
}
void TriangleMesh::SetAttribute(uint8_t bufferIndex, backend::VertexAttribute attributeType, backend::ElementType elementType, uint8_t flags, uint8_t stride, uint32_t offset)
{
    m_declaredAttribute += 1 << attributeType;
    m_attributeArray[attributeType].offset = offset;
    m_attributeArray[attributeType].stride = stride;
    m_attributeArray[attributeType].type = elementType;
    m_attributeArray[attributeType].flags |= flags;
    m_attributeArray[attributeType].buffer = bufferIndex;
}

void TriangleMesh::InitNormals(std::vector<float>& normals)
{
    assert(normals.size() == m_points.size());
    m_normals = std::move(normals);
    normals.clear();

    // TODO Quaterion FLOAT4
    SetAttribute(0, backend::VertexAttribute::TANGENTS, backend::ElementType::FLOAT3, 
        backend::Attribute::FLAG_NORMALIZED, 0, 0); 
}
void TriangleMesh::InitUVs(std::vector<float>& uvs)
{
    assert(uvs.size()/2 == m_points.size()/3);
    m_uvs = std::move(uvs);
    uvs.clear();

    SetAttribute(0, backend::VertexAttribute::UV0, backend::ElementType::FLOAT2, 
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
    m_renderPrimitive->count = m_triIndexs.size() / 3;

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

    m_isRasterGPUInitialized = true;
}
} // namespace geometry
} // namespace lightman
