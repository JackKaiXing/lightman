
#include "geometry/trianglemesh.h"

namespace lightman
{
namespace geometry
{
// ----------------------------------------------------------------------------
TriangleMesh::TriangleMesh(const std::vector<Point> & _points, std::vector<Triangle> & _tris)
{
    // TODO, better memeory Cut-Paste
    copy(_points.begin(), _points.end(), back_inserter(m_points));
    copy(_tris.begin(), _tris.end(), back_inserter(m_tris));
}
} // namespace geometry
} // namespace lightman
