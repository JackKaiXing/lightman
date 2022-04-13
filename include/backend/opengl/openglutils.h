#ifndef _LIGHTMAN_OPENGLUTILS_H
#define _LIGHTMAN_OPENGLUTILS_H

#include "backend/opengl/openglheaders.h"

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
        };

        #ifdef DEBUG
        #   define CHECK_GL_ERROR()
        #else
        #   define CHECK_GL_ERROR() { OpenGLUtils::AssertGLError(__PRETTY_FUNCTION__, __LINE__); }
        #endif
        
        
    }
}
#endif // _LIGHTMAN_OPENGLUTILS_H