#ifndef _LIGHTMAN_OPENGLDRIVERBASE_H
#define _LIGHTMAN_OPENGLDRIVERBASE_H

#include <array>

#include "GL/glcorearb.h"

#include "backend/driverbase.h"
#include "backend/program.h"
#include "backend/opengl/openglplatform.h"
#include "backend/opengl/openglcontext.h"

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        // OpengGL Object Definitions
        // Upper Layer would not aware of these.
        // ----------------------------------------------------------------------------
        struct GLVertexBuffer : public HwVertexBuffer
        {
            struct
            {   
                std::array<GLuint, MAX_VERTEX_ATTRIBUTE_COUNT> buffer{}; // OpenGL VertexBuffer target, i.e. generated from glGenBuffer()
            }gl;  
        };

        struct GLIndexBuffer : public HwIndexBuffer 
        {
            struct
            {
                GLuint buffer{};    // OpenGL IndexBuffer target
            } gl;
        };

        struct GLRenderPrimitive : public HwRenderPrimitive
        {
            OpenGLContext::RenderPrimitive gl;
        };

        struct GLPROGRAM : public HwProgram
        {
            struct
            {
                GLuint shaders[Program::SHADER_TYPE_COUNT];
                GLuint program;
            } gl;
        };  

        class OpengGLDriver : public Driver
        {
        public:
            static backend::Driver* create(backend::OpenGLPlatform* platform, void* sharedGLContext = nullptr) noexcept;
        };
    }
}
#endif // _LIGHTMAN_OPENGLDRIVERBASE_H