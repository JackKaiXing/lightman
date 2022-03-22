#ifndef _LIGHTMAN_TRIANGLEMESH_H
#define _LIGHTMAN_TRIANGLEMESH_H

#include <vector>
#include <cstdint>

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
        // TODO： virtual public？
        class TriangleMesh : public Mesh
        {
        public:
            TriangleMesh(const std::vector<Point> &p, std::vector<Triangle> &m_tris);
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
        private:
            std::vector<Triangle> m_tris;
            std::vector<Point> m_points; // model space position
            std::vector<Normal> m_normals; // vertex normal
            std::vector<Normal> m_triNormals; // triangle normal
            std::vector<UV> m_UVs; // vertex uv
            std::vector<float> m_alphs; // vertex alpha
            std::vector<Color> m_vColors; // vertex color
            BBox m_bBox;
            Transform m_transform;

            bool hasVertexNormal = false;
            bool hasUV = false;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLEMESH_H
