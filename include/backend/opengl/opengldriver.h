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
            struct
            {
                GLuint id = 0;
                GLenum binding = 0;
            } gl;
            backend::BufferUsage usage = {};
        };

        struct GLTexture : public backend::HwTexture
        {
            struct GL {
                GL() noexcept : imported(false), sidecarSamples(1), reserved(0) {}
                GLuint id = 0;          // texture or renderbuffer id
                GLenum target = 0;
                GLenum internalFormat = 0;
                GLuint sidecarRenderBufferMS = 0;  // multi-sample sidecar renderbuffer
                mutable GLsync fence = {};

                // texture parameters go here too
                GLfloat anisotropy = 1.0;
                int8_t baseLevel = 127;
                int8_t maxLevel = -1;
                uint8_t targetIndex = 0;    // optimization: index corresponding to target
                bool imported           : 1;
                uint8_t sidecarSamples  : 4;
                uint8_t reserved        : 3;
            } gl;

            void* platformPImpl = nullptr;
        };

        struct GLRenderTarget : public backend::HwRenderTarget
        {
            struct
            {
                // field ordering to optimize size on 64-bits
                GLTexture* color[HwRenderTarget::MAX_SUPPORTED_RENDER_TARGET_COUNT];
                GLTexture* depth;
                GLTexture* stencil;
                GLuint fbo = 0;
                mutable GLuint fbo_read = 0;
                mutable TargetBufferFlags resolve = TargetBufferFlags::NONE; // attachments in fbo_draw to resolve
                uint8_t samples = 1;
            } gl;
            TargetBufferFlags targets = {};
        };

        class OpenGLDriver : public Driver
        {
        public:
            static backend::Driver* create(backend::OpenGLPlatform* platform, void* sharedGLContext = nullptr) noexcept;
        public:
            #define DECL_DRIVER_API(methodName, paramsDecl, params) void methodName(paramsDecl) override;
            #define DECL_DRIVER_API_RETURN(RetType, methodName, paramsDecl, params) RetType methodName(paramsDecl) override;
            #include "backend/driverapi.inc"
        protected:
            OpenGLDriver(backend::OpenGLPlatform* platform, void* sharedGLContext = nullptr);
        private:
            void framebufferTexture(backend::TargetBufferInfo const& binfo, GLRenderTarget const* rt, GLenum attachment);
            void clearWithRasterPipe(TargetBufferFlags clearFlags, math::Vector4 const& linearColor, GLfloat depth, GLint stencil) noexcept;
            OpenGLPlatform * m_platfrom = nullptr;
        };
    }
}
#endif // _LIGHTMAN_OPENGLDRIVERBASE_H
