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

        enum class TextureUsage : uint8_t
        {
            NONE                = 0x0,
            COLOR_ATTACHMENT    = 0x1,
            DEPTH_ATTACHMENT    = 0x2,
            STENCIL_ATTACHMENT  = 0x4,
            UPLOADABLE          = 0x8,
            SAMPLEABLE          = 0x10,
        };

        enum class TextureFormat : uint16_t
        {
            // 8-bits per element
            R8, R8_SNORM, R8UI, R8I, STENCIL8,

            // 16-bits per element
            R16F, R16UI, R16I,
            RG8, RG8_SNORM, RG8UI, RG8I,
            RGB565,
            RGB9_E5, // 9995 is actually 32 bpp but it's here for historical reasons.
            RGB5_A1,
            RGBA4,
            DEPTH16,

            // 24-bits per element
            RGB8, SRGB8, RGB8_SNORM, RGB8UI, RGB8I,
            DEPTH24,

            // 32-bits per element
            R32F, R32UI, R32I,
            RG16F, RG16UI, RG16I,
            R11F_G11F_B10F,
            RGBA8, SRGB8_A8,RGBA8_SNORM,
            UNUSED, // used to be rgbm
            RGB10_A2, RGBA8UI, RGBA8I,
            DEPTH32F, DEPTH24_STENCIL8, DEPTH32F_STENCIL8,

            // 48-bits per element
            RGB16F, RGB16UI, RGB16I,

            // 64-bits per element
            RG32F, RG32UI, RG32I,
            RGBA16F, RGBA16UI, RGBA16I,

            // 96-bits per element
            RGB32F, RGB32UI, RGB32I,

            // 128-bits per element
            RGBA32F, RGBA32UI, RGBA32I
        };

        enum class SamplerType : uint8_t
        {
            SAMPLER_2D,
            SAMPLER_2D_ARRAY,
            SAMPLER_CUBEMAP
        };

        enum class TargetBufferFlags : uint32_t
        {
            NONE = 0x0u,                            //!< No buffer selected.
            COLOR0 = 0x00000001u,
            COLOR1 = 0x00000002u,
            COLOR2 = 0x00000004u,
            COLOR3 = 0x00000008u,
            COLOR4 = 0x00000010u,
            COLOR5 = 0x00000020u,
            COLOR6 = 0x00000040u,
            COLOR7 = 0x00000080u,
            COLOR_ALL = COLOR0 | COLOR1 | COLOR2 | COLOR3 | COLOR4 | COLOR5 | COLOR6 | COLOR7,
            DEPTH   = 0x10000000u,
            STENCIL = 0x20000000u,
            DEPTH_AND_STENCIL = DEPTH | STENCIL,
            ALL = COLOR_ALL | DEPTH | STENCIL
        };

        inline constexpr TargetBufferFlags getTargetBufferFlagsAt(size_t index) noexcept
        {
            if (index == 0u) return TargetBufferFlags::COLOR0;
            if (index == 1u) return TargetBufferFlags::COLOR1;
            if (index == 2u) return TargetBufferFlags::COLOR2;
            if (index == 3u) return TargetBufferFlags::COLOR3;
            if (index == 4u) return TargetBufferFlags::COLOR4;
            if (index == 5u) return TargetBufferFlags::COLOR5;
            if (index == 6u) return TargetBufferFlags::COLOR6;
            if (index == 7u) return TargetBufferFlags::COLOR7;
            if (index == 8u) return TargetBufferFlags::DEPTH;
            if (index == 9u) return TargetBufferFlags::STENCIL;
            return TargetBufferFlags::NONE;
        }
    }
}
#endif // _LIGHTMAN_DRIVERENUM_H
