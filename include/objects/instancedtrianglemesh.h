#ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
#define _LIGHTMAN_INSTANCEDTRIANGLEMESH_H

#include "math/transform.h"
#include "geometry/trianglemesh.h"
#include "objects/instancedobject.h"

using namespace lightman::math;
using namespace lightman::geometry;

namespace lightman
{
    namespace objects
    {
        // ----------------------------------------------------------------------------
        class InstancedTriangleMesh : public InstancedObject
        {
        public:
            // tranfrom from model space to world space
            Transform m_transform;
            TriangleMesh * m_triMesh;
        };
    } // namespace objects
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H