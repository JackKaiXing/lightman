
#include "backend/opengl/openglutils.h"

#include <iostream>

namespace lightman
{
    namespace backend
    {
        //----------------------------------------------------------------------------
        // OpengGL Utils
        //----------------------------------------------------------------------------
        namespace OpenGLUtils
        {
            const char* getGLError(GLenum error) noexcept {
                const char* string = "unknown";
                switch (error) {
                    case GL_NO_ERROR:
                        string = "GL_NO_ERROR";
                        break;
                    case GL_INVALID_ENUM:
                        string = "GL_INVALID_ENUM";
                        break;
                    case GL_INVALID_VALUE:
                        string = "GL_INVALID_VALUE";
                        break;
                    case GL_INVALID_OPERATION:
                        string = "GL_INVALID_OPERATION";
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        string = "GL_INVALID_FRAMEBUFFER_OPERATION";
                        break;
                    case GL_OUT_OF_MEMORY:
                        string = "GL_OUT_OF_MEMORY";
                        break;
                    default:
                        break;
                }
                return string;
            }

            void AssertGLError(const char* function, size_t line) noexcept
            {
                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                {
                    const char* string = getGLError(error);
                    std::cout << "OpenGL error " << error << " (" << string << ") in \"" << function << "\" at line " << line << std::endl;
                }
            }
        }; 
    }
}