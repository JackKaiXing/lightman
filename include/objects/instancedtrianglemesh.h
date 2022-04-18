#ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
#define _LIGHTMAN_INSTANCEDTRIANGLEMESH_H

#include "math/matrix4x4.h"
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
        void SetTransform(const Matrix4X4& m);
        void SetMesh(std::string name, Mesh* mesh);
        void PrepareForRasterGPU();
        void Draw();
    private:
        // tranfrom from model space to world space
        Matrix4X4 m_transform;
        std::string m_meshName;
        Mesh* m_mesh = nullptr;
    };
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H