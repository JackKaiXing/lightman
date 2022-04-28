#ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
#define _LIGHTMAN_INSTANCEDTRIANGLEMESH_H

#include "math/matrix4x4.h"
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
        InstancedTriangleMesh();
        ~InstancedTriangleMesh();
        void SetTransform(const Matrix4X4& m);
        void SetMaterialInstance(MaterialInstance * mi);
        void SetMesh(std::string name, Mesh* mesh);
        void PrepareForRasterGPU(GPURenderer* renderer);
        void Draw();
        MaterialInstance* GetMaterialInstance();
        void setPVTransform(const Matrix4X4& pvmatrix);
        uint32_t GetProgramIndexBySupportedVertexAttribute();
        bool IsNeedToUpdateProgram();
        void UpdateProgram(HwProgram* program);
    private:
        // tranfrom from model space to world space
        Matrix4X4 m_transform;
        Matrix4X4 m_PVTransform;
        std::string m_meshName;
        TriangleMesh* m_mesh = nullptr;
        MaterialInstance * m_mInstance = nullptr;
        HwProgram* m_program = nullptr;
        bool m_needToUpdateProgram = true;
        bool m_needToUpdateTransform = true;
    };
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDTRIANGLEMESH_H
