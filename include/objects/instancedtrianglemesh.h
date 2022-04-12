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
        InstancedTriangleMesh();
        ~InstancedTriangleMesh();
        Transform& GetTransform();
        void SetMesh(std::string name, Mesh* mesh);
        void PrepareForRasterGPU();
    private:
        // tranfrom from model space to world space
        Transform m_transform;
        std::string m_meshName;
        Mesh* m_mesh = nullptr;
    };
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H