#ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
#define _LIGHTMAN_INSTANCEDTRIANGLEMESH_H

#include "math/matrix4x4.h"
#include "math/vector.h"
#include "geometry/trianglemesh.h"
#include "objects/instancedobject.h"
#include "materials/materialinstance.h"
#include "renderer/renderer.h"

using namespace lightman::math;
using namespace lightman::geometry;

namespace lightman
{
    // ----------------------------------------------------------------------------
    class GPURenderer;
    class InstancedTriangleMesh : public InstancedObject
    {
    public:
        InstancedTriangleMesh(const std::string& name);
        ~InstancedTriangleMesh();
        void SetTransform(const Matrix4X4& m);
        void SetMaterialInstance(MaterialInstance * mi);
        void SetMaterial(Material* mat);
        void SetMesh(std::string name, Mesh* mesh);
        Mesh* GetMesh();
        void PrepareForRasterGPU();
        void Draw();
        MaterialInstance* GetMaterialInstance();
        bool hasMaterial();
        void setPVTransform(const Matrix4X4& pvmatrix, const Vector3& cameraPos);
        uint32_t GetProgramIndexBySupportedVertexAttribute();
        std::string GetName();
    private:
        // tranfrom from model space to world space
        Matrix4X4 m_transform;  // for instance
        Matrix4X4 m_transposeInverseMMatrix; // for normals
        Matrix4X4 m_PVMTransform; // for vertex
        Vector3 m_cameraPos;
        std::string m_name; // name of current instanced triangle mesh
        std::string m_masterName; // name of the master triangle mesh
        TriangleMesh* m_mesh = nullptr;
        MaterialInstance * m_mInstance = nullptr;
        // when material is set, the default mi of this material would be used
        // which is shared between any object who use this material.
        Material* m_material = nullptr;
    };
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
