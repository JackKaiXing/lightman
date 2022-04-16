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

            constexpr inline GLboolean getNormalization(bool normalized) noexcept {
                return GLboolean(normalized ? GL_TRUE : GL_FALSE);
            }
        };

        #ifdef DEBUG
        #   define CHECK_GL_ERROR()
        #else
        #   define CHECK_GL_ERROR() { OpenGLUtils::AssertGLError(__PRETTY_FUNCTION__, __LINE__); }
        #endif
        
        
    }
}
#endif // _LIGHTMAN_OPENGLUTILS_H