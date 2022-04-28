#ifndef _LIGHTMAN_DRIVERENUM_H
#define _LIGHTMAN_DRIVERENUM_H

#include <cstdint>
#include <cstddef>

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        // Shader Program Related Types.
        // ----------------------------------------------------------------------------
        enum class UniformType : uint8_t {
            BOOL,
            BOOL2,
            BOOL3,
            BOOL4,
            FLOAT,
            FLOAT2,
            FLOAT3,
            FLOAT4,
            INT,
            INT2,
            INT3,
            INT4,
            UINT,
            UINT2,
            UINT3,
            UINT4,
            MAT3,   //!< a 3x3 float matrix
            MAT4    //!< a 4x4 float matrix
        };

        enum class Precision : uint8_t {
            LOW,
            MEDIUM,
            HIGH,
            DEFAULT
        };

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
            // do not changed
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
            UV0             = 2, //!< texture coordinates (float2)
            UV1             = 3, //!< texture coordinates (float2)
        };

        // Driver
        enum class BackendType : uint8_t
        {
            OPENGL = 0, // also default
            METAL,
            VULKAN,
        };

        static constexpr size_t SHADER_TYPE_COUNT = 2;
        enum class Shader : uint8_t
        {
            VERTEX = 0,
            FRAGMENT = 1
        };
    }
}
#endif // _LIGHTMAN_DRIVERENUM_H
