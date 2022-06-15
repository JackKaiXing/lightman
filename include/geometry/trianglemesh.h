#ifndef _LIGHTMAN_TRIANGLEMESH_H
#define _LIGHTMAN_TRIANGLEMESH_H

#include <vector>
#include <cstdint>

#include "geometry/triangle.h"
#include "geometry/mesh.h"
#include "geometry/bbox.h"
#include "geometry/normal.h"
#include "math/matrix4x4.h"
#include "color/color.h"

#include "backend/driverbase.h"

using namespace lightman::math;
using namespace lightman::backend;

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        // https://en.wikipedia.org/wiki/Virtual_inheritance 
        class TriangleMesh : virtual public Mesh
        {
        public:
            TriangleMesh(std::vector<unsigned int>& triIndexs, std::vector<float>& points);
            void InitNormals(std::vector<float>& normals);
            void GenerateVertexNormals();
            void InitUVs(std::vector<float>& uvs);
            virtual ~TriangleMesh();
            bool HasNormal() {return m_normals.size()>0;};
            bool HasUV() {return m_uvs.size()>0;};
            MeshType GetMeshType() override {return MeshType::Triangle;};
            void PrepareForRasterGPU() override;
            void Draw(backend::HwProgram * program) override;
            void SetAppliedTransform(const Matrix4X4& mat) override;
            Matrix4X4 GetTransform();
        private:
            void SetAttribute(backend::VertexAttribute attributeType, backend::ElementType elementType, 
                uint8_t flags, uint8_t stride, uint32_t offset);
        private:
            std::vector<unsigned int> m_triIndexs;         // i.e tri0.v0, tri0.v1, tri0.v2, tri1.v0, tri1.v1, tri1.v2, ...
            std::vector<float> m_points;                   // model space position, i.e. p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, ...
            std::vector<float> m_normals;                  // vertex normal
            std::vector<float> m_uvs;                      // vertex uv, i.e. u0, v0, u1, v1, ...
            
            BBox m_bBox;
            Matrix4X4 m_transform;
            Matrix4X4 m_appliedTransform;                  // world to local space

            bool m_isRasterGPUInitialized = false;
            backend::HwRenderPrimitive* m_renderPrimitive = nullptr;
            backend::HwIndexBuffer * m_indexBuffer = nullptr;
            backend::HwVertexBuffer * m_vertexBuffer = nullptr;
            uint16_t m_declaredAttribute = 0;
            backend::AttributeArray m_attributeArray;
            HwBufferObject * m_posBuffetObject = nullptr;
            HwBufferObject * m_normalBuffetObject = nullptr;
            HwBufferObject * m_uvBuffetObject = nullptr;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLEMESH_H
