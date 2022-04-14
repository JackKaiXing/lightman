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
        };

        #ifdef DEBUG
        #   define CHECK_GL_ERROR()
        #else
        #   define CHECK_GL_ERROR() { OpenGLUtils::AssertGLError(__PRETTY_FUNCTION__, __LINE__); }
        #endif
        
        
    }
}
#endif // _LIGHTMAN_OPENGLUTILS_H