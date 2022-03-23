
#include "geometry/trianglemesh.h"

namespace lightman
{
namespace geometry
{
// ----------------------------------------------------------------------------
TriangleMesh::TriangleMesh(const std::vector<Point> & points, const std::vector<Triangle> & tris)
{
    // TODO, better memeory Cut-Paste
    copy(points.begin(), points.end(), back_inserter(m_points));
    copy(tris.begin(), tris.end(), back_inserter(m_tris));
}
void TriangleMesh::InitNormals(const std::vector<Normal> &normals)
{
    // TODO, better memeory Cut-Paste
    copy(normals.begin(), normals.end(), back_inserter(m_normals));
}
void TriangleMesh::InitUVs(const std::vector<UV> &uvs)
{
    // TODO, better memeory Cut-Paste
    copy(uvs.begin(), uvs.end(), back_inserter(m_UVs));
}
} // namespace geometry
} // namespace lightman
