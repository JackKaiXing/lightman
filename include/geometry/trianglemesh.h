#ifndef _LIGHTMAN_TRIANGLEMESH_H
#define _LIGHTMAN_TRIANGLEMESH_H

#include <vector>

#include "geometry/triangle.h"
#include "geometry/point.h"
#include "geometry/mesh.h"
#include "geometry/bbox.h"
#include "geometry/normal.h"
#include "geometry/uv.h"
#include "math/transform.h"
#include "color/color.h"

using namespace lightman::color;
using namespace lightman::math;

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class TriangleMesh : public Mesh
        {
        public:
            std::vector<Triangle> m_tris;
            std::vector<Point> m_points; // model space position
            std::vector<Normal> m_normals; // vertex normal
            std::vector<Normal> m_triNormals; // triangle normal
            std::vector<UV> m_UVs; // vertex uv
            std::vector<float> m_alphs; // vertex alpha
            std::vector<Color> m_vColors; // vertex color
            BBox m_bBox;
            Transform m_transform;
        public:
            virtual ~TriangleMesh()
            {
                m_tris.clear();
                m_points.clear();
                m_normals.clear();
                m_triNormals.clear();
                m_UVs.clear();
                m_alphs.clear();
                m_vColors.clear();
            }
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLEMESH_H