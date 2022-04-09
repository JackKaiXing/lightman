
#include <assert.h>

#include "geometry/trianglemesh.h"

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
}
void TriangleMesh::InitNormals(std::vector<float>& normals)
{
    assert(normals.size() == m_points.size());
    m_normals = std::move(normals);
    normals.clear();
}
void TriangleMesh::InitUVs(std::vector<float>& uvs)
{
    assert(uvs.size()/2 == m_points.size()/3);
    m_uvs = std::move(uvs);
    uvs.clear();
}
void TriangleMesh::preRender()
{
    if(m_renderPrimitive == nullptr)
    {

    }
}
} // namespace geometry
} // namespace lightman
