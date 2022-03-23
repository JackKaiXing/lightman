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
        // https://en.wikipedia.org/wiki/Virtual_inheritance 
        class TriangleMesh : virtual public Mesh
        {
        public:
            TriangleMesh(const std::vector<Point> & points, const std::vector<Triangle> & tris);
            void InitNormals(const std::vector<Normal> &normals);
            void InitUVs(const std::vector<UV> &uvs);
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
            bool hasNormal() {return m_normals.size() > 0;};
            bool hasUV() {return m_UVs.size() > 0;};
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
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLEMESH_H
