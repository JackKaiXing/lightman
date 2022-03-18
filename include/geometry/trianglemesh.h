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
            std::vector<Triangle> m_Tris;
            std::vector<Point> m_Points;
            BBox m_BBox;
        public:
            virtual ~TriangleMesh()
            {
                m_Tris.clear();
                m_Points.clear();
            }
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLEMESH_H