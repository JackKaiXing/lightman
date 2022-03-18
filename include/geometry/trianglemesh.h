#ifndef _LIGHTMAN_TRIANGLEMESH_H
#define _LIGHTMAN_TRIANGLEMESH_H

#include <vector>

#include "geometry/triangle.h"
#include "geometry/point.h"
#include "geometry/mesh.h"

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class TriangleMesh : public Mesh
        {
        public:
            std::vector<Triangle> m_tris;
            std::vector<Point> m_points;
            BBox m_bBox;
        public:
            virtual ~TriangleMesh()
            {
                m_tris.clear();
                m_points.clear();
            }
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLEMESH_H