#ifndef _LIGHTMAN_MESH_H
#define _LIGHTMAN_MESH_H

#include <cstdint>

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

        class Mesh
        {
        public:
            virtual ~Mesh(){};
            virtual MeshType GetMeshType() = 0;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_MESH_H