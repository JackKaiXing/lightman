#ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
#define _LIGHTMAN_INSTANCEDTRIANGLEMESH_H

#include "geometry/transform.h"
#include "geometry/trianglemesh.h"

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class InstancedTriangleMesh
        {
        public:
            // tranfrom from model space to world space
            Transform m_transform;
            TriangleMesh * m_triMesh;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H