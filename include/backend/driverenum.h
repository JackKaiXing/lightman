#ifndef _LIGHTMAN_DRIVERENUM_H
#define _LIGHTMAN_DRIVERENUM_H

#include <cstdint>
#include <cstddef>

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        // C++ Driver Interface Type Definitions
        // Specific Dirver should implement conversions to their own type.
        // ----------------------------------------------------------------------------
        enum class ElementType : uint8_t
        {
            BYTE,
            BYTE2,
            BYTE3,
            BYTE4,
            UBYTE,
            UBYTE2,
            UBYTE3,
            UBYTE4,
            SHORT,
            SHORT2,
            SHORT3,
            SHORT4,
            USHORT,
            USHORT2,
            USHORT3,
            USHORT4,
            INT,
            UINT,
            FLOAT,
            FLOAT2,
            FLOAT3,
            FLOAT4,
        };

        enum class BufferUsage : uint8_t
        {
            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
            STATIC,     
            DYNAMIC,    
            STREAM,     
        };
        enum class PrimitiveType : uint8_t {
            POINTS      = 0,
            LINES       = 1,
            TRIANGLES   = 4,
            NONE        = 0xFF
        };

        // https://www.khronos.org/registry/OpenGL-Refpages/es3/html/glGet.xhtml
        static constexpr size_t MAX_VERTEX_ATTRIBUTE_COUNT = 16;

        // Driver
        enum class BackendType : uint8_t
        {
            OPENGL = 0, // also default
            METAL,
            VULKAN,
        };
    }
}
#endif // _LIGHTMAN_DRIVERENUM_H