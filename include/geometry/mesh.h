#ifndef _LIGHTMAN_MESH_H
#define _LIGHTMAN_MESH_H

#include <cstdint>
#include "utils/ref.h"

using namespace lightman::utils;
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
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_MESH_H