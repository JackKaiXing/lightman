#ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
#define _LIGHTMAN_INSTANCEDTRIANGLEMESH_H

#include "math/transform.h"
#include "geometry/trianglemesh.h"
#include "objects/instancedobject.h"

using namespace lightman::math;
using namespace lightman::geometry;

namespace lightman
{
    // ----------------------------------------------------------------------------
    class InstancedTriangleMesh : public InstancedObject
    {
    public:
        Transform& getTransform();
        void setMeshName(std::string _mesh);
    private:
        // tranfrom from model space to world space
        Transform m_transform;
        std::string m_triMeshName;
    };
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H