#ifndef _LIGHTMAN_MESH_H
#define _LIGHTMAN_MESH_H

#include <cstdint>
#include "utils/ref.h"
#include "backend/driverbase.h"
#include "math/matrix4x4.h"

using namespace lightman::utils;
using namespace lightman::math;

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        enum class MeshType: uint8_t
        {
            Triangle = 0,
            Quad
        };

        class Mesh : virtual public Ref
        {
        public:
            virtual ~Mesh(){};
            virtual MeshType GetMeshType() = 0;
            virtual void PrepareForRasterGPU() = 0;
            virtual void Draw(backend::HwProgram * program) = 0;
            virtual void SetAppliedTransform(const Matrix4X4& mat) = 0;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_MESH_H
