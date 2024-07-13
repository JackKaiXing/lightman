#ifndef _LIGHTMAN_OPENGLUTILS_H
#define _LIGHTMAN_OPENGLUTILS_H

#include "backend/opengl/openglheaders.h"
#include "backend/driverenum.h"

namespace lightman
{
    namespace backend
    {
        //----------------------------------------------------------------------------
        // OpengGL Utils
        //----------------------------------------------------------------------------
        namespace OpenGLUtils
        {
            void AssertGLError(const char* function, size_t line) noexcept;

            const char* getGLError(GLenum error) noexcept;

            constexpr inline GLenum getBufferUsage(backend::BufferUsage usage) noexcept
            {
                switch (usage) {
                case backend::BufferUsage::STATIC:
                    return GL_STATIC_DRAW;
                case backend::BufferUsage::DYNAMIC:
                case backend::BufferUsage::STREAM:
                    return GL_DYNAMIC_DRAW;
                }
            }

            constexpr inline GLenum getBufferBindingType(backend::BufferObjectBinding bindingType) noexcept
            {
                switch (bindingType)
                {
                    case backend::BufferObjectBinding::VERTEX:
                        return GL_ARRAY_BUFFER;
                    case backend::BufferObjectBinding::UNIFORM:
                        return GL_UNIFORM_BUFFER;
                }
            }

            constexpr inline GLuint getComponentCount(backend::ElementType type) noexcept {
                using ElementType = backend::ElementType;
                switch (type) {
                    case ElementType::BYTE:
                    case ElementType::UBYTE:
                    case ElementType::SHORT:
                    case ElementType::USHORT:
                    case ElementType::INT:
                    case ElementType::UINT:
                    case ElementType::FLOAT:
                        return 1;
                    case ElementType::FLOAT2:
                    case ElementType::BYTE2:
                    case ElementType::UBYTE2:
                    case ElementType::SHORT2:
                    case ElementType::USHORT2:
                        return 2;
                    case ElementType::FLOAT3:
                    case ElementType::BYTE3:
                    case ElementType::UBYTE3:
                    case ElementType::SHORT3:
                    case ElementType::USHORT3:
                        return 3;
                    case ElementType::FLOAT4:
                    case ElementType::BYTE4:
                    case ElementType::UBYTE4:
                    case ElementType::SHORT4:
                    case ElementType::USHORT4:
                        return 4;
                }
            }

            constexpr inline GLenum getComponentType(backend::ElementType type) noexcept {
                using ElementType = backend::ElementType;
                switch (type) {
                    case ElementType::BYTE:
                    case ElementType::BYTE2:
                    case ElementType::BYTE3:
                    case ElementType::BYTE4:
                        return GL_BYTE;
                    case ElementType::UBYTE:
                    case ElementType::UBYTE2:
                    case ElementType::UBYTE3:
                    case ElementType::UBYTE4:
                        return GL_UNSIGNED_BYTE;
                    case ElementType::SHORT:
                    case ElementType::SHORT2:
                    case ElementType::SHORT3:
                    case ElementType::SHORT4:
                        return GL_SHORT;
                    case ElementType::USHORT:
                    case ElementType::USHORT2:
                    case ElementType::USHORT3:
                    case ElementType::USHORT4:
                        return GL_UNSIGNED_SHORT;
                    case ElementType::INT:
                        return GL_INT;
                    case ElementType::UINT:
                        return GL_UNSIGNED_INT;
                    case ElementType::FLOAT:
                    case ElementType::FLOAT2:
                    case ElementType::FLOAT3:
                    case ElementType::FLOAT4:
                        return GL_FLOAT;
                }
            }

            constexpr inline GLboolean getNormalization(bool normalized) noexcept
            {
                return GLboolean(normalized ? GL_TRUE : GL_FALSE);
            }

            constexpr inline GLenum GetInternalFormat(backend::TextureFormat format) noexcept 
            {
                switch (format) {
                    // 8-bits per element
                    case TextureFormat::R8:                return GL_R8;
                    case TextureFormat::R8_SNORM:          return GL_R8_SNORM;
                    case TextureFormat::R8UI:              return GL_R8UI;
                    case TextureFormat::R8I:               return GL_R8I;
                    case TextureFormat::STENCIL8:          return GL_STENCIL_INDEX8;

                    // 16-bits per element
                    case TextureFormat::R16F:              return GL_R16F;
                    case TextureFormat::R16UI:             return GL_R16UI;
                    case TextureFormat::R16I:              return GL_R16I;
                    case TextureFormat::RG8:               return GL_RG8;
                    case TextureFormat::RG8_SNORM:         return GL_RG8_SNORM;
                    case TextureFormat::RG8UI:             return GL_RG8UI;
                    case TextureFormat::RG8I:              return GL_RG8I;
                    case TextureFormat::RGB565:            return GL_RGB565;
                    case TextureFormat::RGB5_A1:           return GL_RGB5_A1;
                    case TextureFormat::RGBA4:             return GL_RGBA4;
                    case TextureFormat::DEPTH16:           return GL_DEPTH_COMPONENT16;

                    // 24-bits per element
                    case TextureFormat::RGB8:              return GL_RGB8;
                    case TextureFormat::SRGB8:             return GL_SRGB8;
                    case TextureFormat::RGB8_SNORM:        return GL_RGB8_SNORM;
                    case TextureFormat::RGB8UI:            return GL_RGB8UI;
                    case TextureFormat::RGB8I:             return GL_RGB8I;
                    case TextureFormat::DEPTH24:           return GL_DEPTH_COMPONENT24;

                    // 32-bits per element
                    case TextureFormat::R32F:              return GL_R32F;
                    case TextureFormat::R32UI:             return GL_R32UI;
                    case TextureFormat::R32I:              return GL_R32I;
                    case TextureFormat::RG16F:             return GL_RG16F;
                    case TextureFormat::RG16UI:            return GL_RG16UI;
                    case TextureFormat::RG16I:             return GL_RG16I;
                    case TextureFormat::R11F_G11F_B10F:    return GL_R11F_G11F_B10F;
                    case TextureFormat::RGB9_E5:           return GL_RGB9_E5;
                    case TextureFormat::RGBA8:             return GL_RGBA8;
                    case TextureFormat::SRGB8_A8:          return GL_SRGB8_ALPHA8;
                    case TextureFormat::RGBA8_SNORM:       return GL_RGBA8_SNORM;
                    case TextureFormat::RGB10_A2:          return GL_RGB10_A2;
                    case TextureFormat::RGBA8UI:           return GL_RGBA8UI;
                    case TextureFormat::RGBA8I:            return GL_RGBA8I;
                    case TextureFormat::DEPTH32F:          return GL_DEPTH_COMPONENT32F;
                    case TextureFormat::DEPTH24_STENCIL8:  return GL_DEPTH24_STENCIL8;
                    case TextureFormat::DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;

                    // 48-bits per element
                    case TextureFormat::RGB16F:            return GL_RGB16F;
                    case TextureFormat::RGB16UI:           return GL_RGB16UI;
                    case TextureFormat::RGB16I:            return GL_RGB16I;

                    // 64-bits per element
                    case TextureFormat::RG32F:             return GL_RG32F;
                    case TextureFormat::RG32UI:            return GL_RG32UI;
                    case TextureFormat::RG32I:             return GL_RG32I;
                    case TextureFormat::RGBA16F:           return GL_RGBA16F;
                    case TextureFormat::RGBA16UI:          return GL_RGBA16UI;
                    case TextureFormat::RGBA16I:           return GL_RGBA16I;

                    // 96-bits per element
                    case TextureFormat::RGB32F:            return GL_RGB32F;
                    case TextureFormat::RGB32UI:           return GL_RGB32UI;
                    case TextureFormat::RGB32I:            return GL_RGB32I;

                    // 128-bits per element
                    case TextureFormat::RGBA32F:           return GL_RGBA32F;
                    case TextureFormat::RGBA32UI:          return GL_RGBA32UI;
                    case TextureFormat::RGBA32I:           return GL_RGBA32I;

                    case TextureFormat::UNUSED:
                        return 0;
                }
            }

            constexpr inline GLenum getFormat(backend::PixelDataFormat format) noexcept
            {
                using PixelDataFormat = backend::PixelDataFormat;
                switch (format)
                {
                    case PixelDataFormat::R:                return GL_RED;
                    case PixelDataFormat::RG:               return GL_RG;
                    case PixelDataFormat::RGB:              return GL_RGB;
                    case PixelDataFormat::RGBA:             return GL_RGBA;
                }
            }

            constexpr inline GLenum getType(backend::PixelDataType type) noexcept
            {
                using PixelDataType = backend::PixelDataType;
                switch (type)
                {
                    case PixelDataType::UBYTE:                return GL_UNSIGNED_BYTE;
                    case PixelDataType::BYTE:                 return GL_BYTE;
                    case PixelDataType::USHORT:               return GL_UNSIGNED_SHORT;
                    case PixelDataType::SHORT:                return GL_SHORT;
                    case PixelDataType::UINT:                 return GL_UNSIGNED_INT;
                    case PixelDataType::INT:                  return GL_INT;
                    case PixelDataType::FLOAT:                return GL_FLOAT;
                }
            }
        };

        #ifdef DEBUG
            #define CHECK_GL_ERROR()
        #else
            #define CHECK_GL_ERROR() { OpenGLUtils::AssertGLError(__FUNCTION__, __LINE__); }
        #endif
        
        
    }
}
#endif // _LIGHTMAN_OPENGLUTILS_H
