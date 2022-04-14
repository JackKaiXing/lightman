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
            UBYTE,
            SHORT,
            USHORT,
            INT,
            UINT,
            FLOAT,
        };

        enum class BufferObjectBinding : uint8_t {
            VERTEX,
            UNIFORM
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
        enum VertexAttribute : uint8_t {
            POSITION        = 0, //!< XYZ position (float3)
            TANGENTS        = 1, //!< tangent, bitangent and normal, encoded as a quaternion (float4)
            COLOR           = 2, //!< vertex color (float4)
            UV0             = 3, //!< texture coordinates (float2)
            UV1             = 4, //!< texture coordinates (float2)
        };

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