#ifndef _LIGHTMAN_OPENGLDRIVERBASE_H
#define _LIGHTMAN_OPENGLDRIVERBASE_H

#include <array>

#include "backend/opengl/openglheaders.h"

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

        struct GLBufferObject : public backend::HwBufferObject {
            struct {
                GLuint id = 0;
                GLenum binding = 0;
            } gl;
            uint32_t base = 0;
            uint32_t size = 0;
            backend::BufferUsage usage = {};
        };

        class OpengGLDriver : public Driver
        {
        public:
            static backend::Driver* create(backend::OpenGLPlatform* platform, void* sharedGLContext = nullptr) noexcept;
        public:
            #define DECL_DRIVER_API(methodName, paramsDecl, params) void methodName(paramsDecl) override;
            #define DECL_DRIVER_API_RETURN(RetType, methodName, paramsDecl, params) RetType methodName(paramsDecl) override;
            #include "backend/driverapi.inc"
        protected:
            OpengGLDriver(backend::OpenGLPlatform* platform, void* sharedGLContext = nullptr);
        private:
            OpenGLPlatform * m_platfrom = nullptr;
        };
    }
}
#endif // _LIGHTMAN_OPENGLDRIVERBASE_H