
#include "backend/opengl/opengldriver.h"
#include "backend/opengl/openglutils.h"

#include <iostream>

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        backend::Driver* OpengGLDriver::create(backend::OpenGLPlatform* platform, void* sharedGLContext) noexcept
        {
            return new OpengGLDriver(platform, sharedGLContext);
        }
        OpengGLDriver::OpengGLDriver(backend::OpenGLPlatform* platform, void* sharedGLContext)
        {
            m_platfrom = platform;
        }
        backend::HwSwapChain* OpengGLDriver::CreateSwapChain(void* nativeWindow)
        {
            backend::HwSwapChain* swapchain = new backend::HwSwapChain();
            swapchain->swapchain = m_platfrom->CreateSwainChain(nativeWindow);
            return swapchain;
        }
        void OpengGLDriver::makeCurrent(backend::HwSwapChain* swapchain)
        {
            m_platfrom->MakeCurrent(swapchain->swapchain);
        }
        void OpengGLDriver::Commit(backend::HwSwapChain* swapchain)
        {
            m_platfrom->Commit(swapchain->swapchain);
        }
        HwRenderPrimitive* OpengGLDriver::createRenderPrimitive()
        {
            GLRenderPrimitive* result = new GLRenderPrimitive();
            glGenVertexArrays(1, &result->gl.vao);
            CHECK_GL_ERROR()
            return result;
        }
        backend::HwVertexBuffer* OpengGLDriver::createVertexBuffer(
            uint8_t bufferCount,
            uint8_t attributeCount,
            uint32_t vertexCount,
            const backend::AttributeArray& attributes)
        {
            assert(attributes.size() != MAX_VERTEX_ATTRIBUTE_COUNT);

            GLVertexBuffer* result = new GLVertexBuffer();
            result->attributes = attributes;
            result->vertexCount = vertexCount;
            for (size_t i = 0; i < MAX_VERTEX_ATTRIBUTE_COUNT; i++)
            {
                if(result->attributes[i].buffer != Attribute::BUFFER_UNUSED)
                {

                }
            }
        }
        backend::HwBufferObject* OpengGLDriver::createBufferObject(
            uint32_t byteCount, 
            backend::BufferObjectBinding bindingType,
            backend::BufferUsage usage)
        {
            return nullptr;
        }
    }
}