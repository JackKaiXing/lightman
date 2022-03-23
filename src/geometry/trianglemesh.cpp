
#include <assert.h>

#include "geometry/trianglemesh.h"

namespace lightman
{
namespace geometry
{
// ----------------------------------------------------------------------------
TriangleMesh::TriangleMesh(const std::vector<Point> & points, const std::vector<Triangle> & tris)
{
    assert(points.size() > 3 && tris.size() > 0);

    // TODO, better memeory Cut-Paste
    copy(points.begin(), points.end(), back_inserter(m_points));
    copy(tris.begin(), tris.end(), back_inserter(m_tris));
}
void TriangleMesh::InitNormals(const std::vector<Normal> &normals)
{
    assert(normals.size() == m_points.size());

    // TODO, better memeory Cut-Paste
    copy(normals.begin(), normals.end(), back_inserter(m_normals));
}
void TriangleMesh::InitUVs(const std::vector<UV> &uvs)
{
    assert(uvs.size() == m_points.size());

    // TODO, better memeory Cut-Paste
    copy(uvs.begin(), uvs.end(), back_inserter(m_UVs));
}
} // namespace geometry
} // namespace lightman
