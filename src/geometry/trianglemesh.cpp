
#include <assert.h>

#include "geometry/trianglemesh.h"

namespace lightman
{
namespace geometry
{
// ----------------------------------------------------------------------------
TriangleMesh::TriangleMesh(std::vector<Point> & points, std::vector<Triangle> & tris)
{
    assert(points.size() > 3 && tris.size() > 0);

    // fast copy, https://en.cppreference.com/w/cpp/utility/move
    m_points = std::move(points);
    m_tris = std::move(tris);
}
void TriangleMesh::InitNormals(std::vector<Normal> &normals)
{
    assert(normals.size() == m_points.size());
    m_normals = std::move(normals);
}
void TriangleMesh::InitUVs(std::vector<UV> &uvs)
{
    assert(uvs.size() == m_points.size());
    m_UVs = std::move(uvs);
}
void TriangleMesh::preRender()
{
    
}
} // namespace geometry
} // namespace lightman
