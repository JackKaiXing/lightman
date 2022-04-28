#ifndef _LIGHTMAN_OPENGLDRIVERBASE_H
#define _LIGHTMAN_OPENGLDRIVERBASE_H

#include <array>

#include "backend/opengl/openglheaders.h"

#include "backend/driverbase.h"
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
                std::array<GLuint, MAX_VERTEX_ATTRIBUTE_COUNT> buffers{}; // OpenGL VertexBuffer target, i.e. generated from glGenBuffer()
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

        struct GLProgram : public HwProgram
        {
            struct
            {
                GLuint shaders[backend::SHADER_TYPE_COUNT];
                GLuint program;
            } gl;

            void LoadShaders(const std::string& VertexShaderCode, const std::string& FragmentShaderCode);
        }; 

        struct GLBufferObject : public backend::HwBufferObject {
            struct {
                GLuint id = 0;
                GLenum binding = 0;
            } gl;
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