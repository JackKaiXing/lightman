#ifndef _LIGHTMAN_OPENGLCONTEXT_H
#define _LIGHTMAN_OPENGLCONTEXT_H

#include <array>

#include "backend/driverbase.h"

namespace lightman
{
    namespace backend
    {
        class OpenGLContext
        {
        public:
            struct RenderPrimitive
            {
                // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
                // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
                GLuint vao = 0;             // vertex array object
                GLuint elementArray = 0;    // index array buffer object
            };
        };
    }
}
#endif // _LIGHTMAN_OPENGLCONTEXT_H