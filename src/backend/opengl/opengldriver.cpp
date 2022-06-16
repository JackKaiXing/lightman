
#include "backend/opengl/opengldriver.h"
#include "backend/opengl/openglutils.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace lightman
{
    namespace backend
    {
        void GLProgram::LoadShaders(const std::string& VertexShaderCode, const std::string& FragmentShaderCode)
        {
            GLuint& VertexShaderID = gl.shaders[uint32_t(backend::Shader::VERTEX)];
            GLuint& FragmentShaderID = gl.shaders[uint32_t(backend::Shader::FRAGMENT)];
            
            VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

            GLint Result = GL_FALSE;
            int InfoLogLength;

            // Compile Vertex Shader
            char const * VertexSourcePointer = VertexShaderCode.c_str();
            glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
            glCompileShader(VertexShaderID);
            
            // Check Vertex Shader
            glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                printf("%s\n", &VertexShaderErrorMessage[0]);
            }
            
            // Compile Fragment Shader
            char const * FragmentSourcePointer = FragmentShaderCode.c_str();
            glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
            glCompileShader(FragmentShaderID);

            // Check Fragment Shader
            glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                printf("%s\n", &FragmentShaderErrorMessage[0]);
            }
            
            // Link the program
            printf("Linking program\n");
            gl.program = glCreateProgram();
            glAttachShader(gl.program, VertexShaderID);
            glAttachShader(gl.program, FragmentShaderID);
            glLinkProgram(gl.program);

            // Check the program
            glGetProgramiv(gl.program, GL_LINK_STATUS, &Result);
            glGetProgramiv(gl.program, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(gl.program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf("%s\n", &ProgramErrorMessage[0]);
            }
            CHECK_GL_ERROR();
        }
        // ----------------------------------------------------------------------------
        backend::Driver* OpenGLDriver::create(backend::OpenGLPlatform* platform, void* sharedGLContext) noexcept
        {
            return new OpenGLDriver(platform, sharedGLContext);
        }
        OpenGLDriver::OpenGLDriver(backend::OpenGLPlatform* platform, void* sharedGLContext)
        {
            m_platfrom = platform;
        }
        backend::HwSwapChain* OpenGLDriver::CreateSwapChain(void* nativeWindow)
        {
            backend::HwSwapChain* swapchain = new backend::HwSwapChain();
            swapchain->swapchain = m_platfrom->CreateSwainChain(nativeWindow);
            return swapchain;
        }
        void OpenGLDriver::makeCurrent(backend::HwSwapChain* swapchain)
        {
            m_platfrom->MakeCurrent(swapchain->swapchain);
        }
        void OpenGLDriver::Commit(backend::HwSwapChain* swapchain)
        {
            m_platfrom->Commit(swapchain->swapchain);
        }
        HwRenderPrimitive* OpenGLDriver::createRenderPrimitive()
        {
            GLRenderPrimitive* result = new GLRenderPrimitive();
            glGenVertexArrays(1, &result->gl.vao);
            CHECK_GL_ERROR();
            return result;
        }
        HwVertexBuffer* OpenGLDriver::createVertexBuffer(
            uint8_t bufferCount,
            uint8_t attributeCount,
            uint32_t vertexCount,
            const backend::AttributeArray& attributes)
        {
            assert(attributes.size() == MAX_VERTEX_ATTRIBUTE_COUNT);

            GLVertexBuffer* result = new GLVertexBuffer();
            std::copy(attributes.begin(), attributes.end(), result->attributes.begin());
            result->vertexCount = vertexCount;
            
            return result;
        }
        HwBufferObject* OpenGLDriver::createBufferObject(
            uint32_t byteCount, 
            backend::BufferObjectBinding bindingType,
            backend::BufferUsage usage)
        {
            GLBufferObject * result = new GLBufferObject();
            result->usage = usage;
            result->byteCount = byteCount;
            result->gl.binding = OpenGLUtils::getBufferBindingType(bindingType);

            glBindVertexArray(0);
            glGenBuffers(1, &result->gl.id);
            glBindBuffer(result->gl.binding, result->gl.id);
            glBufferData(result->gl.binding, byteCount, nullptr, OpenGLUtils::getBufferUsage(usage));
            CHECK_GL_ERROR();

            return result;
        }
        void OpenGLDriver::updateBufferObject(backend::HwBufferObject* boh, void* data, size_t dataSize, uint32_t byteOffset)
        {
            GLBufferObject* bo = static_cast<GLBufferObject*>(boh);

            assert(dataSize + byteOffset <= bo->byteCount);

            if (bo->gl.binding == GL_UNIFORM_BUFFER)
            {
                // TODO check byteoffset and aligment
                glBufferData(bo->gl.binding, dataSize, data, OpenGLUtils::getBufferUsage(bo->usage));
            }
            else{
                if(bo->gl.binding == GL_ARRAY_BUFFER)
                {
                    glBindVertexArray(0);
                }
                glBindBuffer(bo->gl.binding, bo->gl.id);
                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
                if (byteOffset == 0 && dataSize == bo->byteCount) {
                    // TODO Performance test 
                    glBufferData(bo->gl.binding, dataSize, data, OpenGLUtils::getBufferUsage(bo->usage));
                } else {
                    glBufferSubData(bo->gl.binding, byteOffset, dataSize, data);
                }
            }
            CHECK_GL_ERROR();
        }
        void OpenGLDriver::setVertexBufferObject(backend::HwVertexBuffer* vbh,
            uint32_t index, backend::HwBufferObject* bufferObject)
        {
            GLVertexBuffer* vb = static_cast<GLVertexBuffer *>(vbh);
            GLBufferObject* bo = static_cast<GLBufferObject *>(bufferObject);

            assert(bo->gl.binding == GL_ARRAY_BUFFER);
            if (vb->gl.buffers[index] != bo->gl.id)
            {
                vb->gl.buffers[index] = bo->gl.id;
            }
            CHECK_GL_ERROR();

        }
        HwIndexBuffer* OpenGLDriver::createIndexBuffer(backend::ElementType elementType,
            uint32_t indexCount, backend::BufferUsage usage)
        {
            GLIndexBuffer* result = new GLIndexBuffer();
            result->elementSize = static_cast<uint8_t>(getElementTypeSize(elementType));

            GLsizeiptr size = result->elementSize * indexCount;
            glGenBuffers(1, &result->gl.buffer);
            // unbind, https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->gl.buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, OpenGLUtils::getBufferUsage(usage));
            CHECK_GL_ERROR();

            return result;
        }
        void OpenGLDriver::updateIndexBuffer(backend::HwIndexBuffer* ibh,
            void* data, size_t dataSize, uint32_t byteOffset)
        {   
            if(ibh)
            {
                GLIndexBuffer* ib = static_cast<GLIndexBuffer *>(ibh);
                assert(ib->elementSize == 2 || ib->elementSize == 4);
                glBindVertexArray(0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->gl.buffer);
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, byteOffset, dataSize, data);
                CHECK_GL_ERROR();
            }
        }
        void OpenGLDriver::setRenderPrimitiveBuffer(backend::HwRenderPrimitive* rph,
            backend::HwVertexBuffer* vbh, backend::HwIndexBuffer* ibh)
        {
            if(rph && vbh && ibh)
            {
                // https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const
                GLRenderPrimitive* const rp = static_cast<GLRenderPrimitive*>(rph);
                const GLVertexBuffer* vb = static_cast<const GLVertexBuffer*>(vbh);
                const GLIndexBuffer* ib = static_cast<const GLIndexBuffer*>(ibh);

                assert(ib->elementSize == 2 || ib->elementSize == 4);

                glBindVertexArray(rp->gl.vao);
                rp->gl.indicesType = ib->elementSize == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;

                // bing vertex buffer
                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
                for (size_t i = 0, n = vb->attributes.size(); i < n; i++)
                {
                    const auto& attribute = vb->attributes[i];
                    const uint8_t bi = attribute.buffer;
                    if (bi != Attribute::BUFFER_UNUSED && vb->gl.buffers[bi] != 0)
                    {
                        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
                        glBindBuffer(GL_ARRAY_BUFFER, vb->gl.buffers[bi]);
                        if (attribute.flags & Attribute::FLAG_INTEGER_TARGET)
                        {
                            glVertexAttribIPointer(GLuint(i),
                                OpenGLUtils::getComponentCount(attribute.type),
                                OpenGLUtils::getComponentType(attribute.type),
                                attribute.stride,
                                (void*) uintptr_t(attribute.offset));
                        }
                        else
                        {
                            glVertexAttribPointer(GLuint(i),
                                OpenGLUtils::getComponentCount(attribute.type),
                                OpenGLUtils::getComponentType(attribute.type),
                                OpenGLUtils::getNormalization(attribute.flags & Attribute::FLAG_NORMALIZED),
                                attribute.stride,
                                (void*) uintptr_t(attribute.offset));
                        }
                        
                        glEnableVertexAttribArray(GLuint(i));
                    }
                    else
                    {
                        glDisableVertexAttribArray(GLuint(i));
                    }
                }

                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
                // this records the index buffer into the currently bound VAO
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->gl.buffer);

                // unbind vertex array
                glBindVertexArray(0);
            }
            CHECK_GL_ERROR();
        }

        void OpenGLDriver::draw(backend::HwProgram * program, backend::HwRenderPrimitive* rph)
        {
            GLRenderPrimitive* rp = static_cast<GLRenderPrimitive *>(rph);
            GLProgram * glprogram = static_cast<GLProgram *>(program);

            glUseProgram(glprogram->gl.program);

            glBindVertexArray(rp->gl.vao);
            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawRangeElements.xhtml
            glDrawRangeElements(GLenum(rp->type), rp->minIndex, rp->maxIndex, rp->count,
                rp->gl.indicesType, reinterpret_cast<const void*>(rp->offset));
            CHECK_GL_ERROR();
        }

        backend::HwProgram* OpenGLDriver::createProgram(const std::string& vertexShader, const std::string& fragShader,
            UniformBlockInfo ubInfo)
        {
            GLProgram* result = new GLProgram();
            result->LoadShaders(vertexShader, fragShader);
            
            for (GLuint binding = 0, n = ubInfo.size(); binding < n; binding++)
            {
                auto const& name = ubInfo[binding];
                if (!name.empty())
                {
                    GLint index = glGetUniformBlockIndex(result->gl.program, name.c_str());
                    if (index >= 0 && index != GL_INVALID_INDEX) {
                        glUniformBlockBinding(result->gl.program, GLuint(index), binding);
                    }
                    CHECK_GL_ERROR();
                }
            }
            
            return result;
        }
        void OpenGLDriver::bindUniformBuffer(uint32_t index, backend::HwBufferObject* ubh)
        {
            GLBufferObject* ub = static_cast<GLBufferObject *>(ubh);
            assert(ub->gl.binding == GL_UNIFORM_BUFFER);
            glBindBufferRange(ub->gl.binding, GLuint(index), ub->gl.id, 0, ub->byteCount);
            CHECK_GL_ERROR();
        }
        backend::HwTexture* OpenGLDriver::createTexture(
            backend::SamplerType target,
            uint8_t levels,
            backend::TextureFormat format,
            uint8_t samples,
            uint32_t width,
            uint32_t height,
            uint32_t depth,
            backend::TextureUsage usage)
        {
            GLTexture* result = new GLTexture();
            result->samples = samples;
            result->width = width;
            result->height = height;
            result->depth = depth;
            result->levels = levels;
            result->format = format;
            result->usage = usage;

            if (usage == backend::TextureUsage::SAMPLEABLE)
            {
                glGenTextures(1, &result->gl.id);

                result->gl.internalFormat = OpenGLUtils::GetInternalFormat(format);
                assert(result->gl.internalFormat);

                switch (target)
                {
                case SamplerType::SAMPLER_2D:
                    result->gl.target = GL_TEXTURE_2D;
                    break;
                
                default:
                    assert(0); // TODO other types
                }

                if (result->samples > 1)
                {
                    // TODO MulitSamples
                }

                glBindTexture(result->gl.target, result->gl.id);
                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glActiveTexture.xhtml
                switch (result->gl.target)
                {
                case GL_TEXTURE_2D:
                    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
                    glTexStorage2D(result->gl.target, GLsizei(result->levels), result->gl.internalFormat,
                        GLsizei(result->width), GLsizei(result->height));
                    break;
                
                default:
                    assert(0); // TODO other types
                }
                glBindTexture(result->gl.target, 0);
            }
            else
            {
                // https://www.khronos.org/opengl/wiki/Renderbuffer_Object
                // http://www.songho.ca/opengl/gl_fbo.html#renderbuffer
                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenRenderbuffers.xhtml
                assert((uint8_t)usage & ((uint8_t)TextureUsage::COLOR_ATTACHMENT + (uint8_t)TextureUsage::DEPTH_ATTACHMENT + (uint8_t)TextureUsage::STENCIL_ATTACHMENT) );

                result->gl.internalFormat = OpenGLUtils::GetInternalFormat(format);
                result->gl.target = GL_RENDERBUFFER;
                glGenRenderbuffers(1, &result->gl.id);
                glBindRenderbuffer(GL_RENDERBUFFER, result->gl.id);
                if (result->samples > 1)
                {
                    // TODO MulitSamples
                }
                else
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, result->gl.internalFormat, result->width, result->height);
                }
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
            }
            
            CHECK_GL_ERROR();
            return result;
        }
        void OpenGLDriver::framebufferTexture(backend::TargetBufferInfo const& binfo,
            GLRenderTarget const* rt, GLenum attachment)
        {
            GLTexture* t = static_cast<GLTexture*>(binfo.m_tex);
            glBindFramebuffer(GL_FRAMEBUFFER, rt->gl.fbo);
            
            switch (t->target) {
                case backend::SamplerType::SAMPLER_2D:
                    {
                        if (binfo.m_tex->usage == backend::TextureUsage::SAMPLEABLE)
                        {
                            // https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
                            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, t->gl.id, binfo.m_level);
                        }
                        else
                        {
                            glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, t->gl.id);
                        }
                    }
                    break;
                    
                default:
                    break;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            CHECK_GL_ERROR();
        }
        backend::HwRenderTarget* OpenGLDriver::createDefaultRenderTarget(int)
        {
            GLRenderTarget * result = new GLRenderTarget();
            result->height = 0; // TODO
            result->width = 0;
            result->gl.samples = 1; // NO MULTIPLE SAMPLING
            result->targets = TargetBufferFlags((uint32_t)TargetBufferFlags::COLOR0 | (uint32_t)TargetBufferFlags::DEPTH_AND_STENCIL);
            
            result->gl.fbo = 0;
            
            return result;
        }
        backend::HwRenderTarget* OpenGLDriver::createRenderTarget(
            backend::TargetBufferFlags targetsFlags,
            uint32_t width,
            uint32_t height,
            uint8_t samples,
            backend::MRT color,
            backend::TargetBufferInfo depth,
            backend::TargetBufferInfo stencil)
        {
            GLRenderTarget * result = new GLRenderTarget();
            result->height = height;
            result->width = width;
            result->gl.samples = samples;
            result->targets = targetsFlags;

            glGenFramebuffers(1, &result->gl.fbo);
            
            if ((uint32_t)(backend::TargetBufferFlags::COLOR_ALL) & (uint32_t)result->targets)
            {
                GLenum bufs[HwRenderTarget::MAX_SUPPORTED_RENDER_TARGET_COUNT] = { GL_NONE };
                for (size_t i = 0; i < HwRenderTarget::MAX_SUPPORTED_RENDER_TARGET_COUNT; i++)
                {
                    if((uint32_t)(getTargetBufferFlagsAt(i)) & (uint32_t)result->targets)
                    {
                        result->gl.color[i] = static_cast<GLTexture*>(color[i].m_tex);
                        framebufferTexture(color[i], result, GL_COLOR_ATTACHMENT0 + i);
                        bufs[i] = GL_COLOR_ATTACHMENT0 + i;
                    }
                }
                // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawBuffers.xhtml
                glBindFramebuffer(GL_FRAMEBUFFER, result->gl.fbo);
                glDrawBuffers(HwRenderTarget::MAX_SUPPORTED_RENDER_TARGET_COUNT, bufs);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                CHECK_GL_ERROR();
            }

            if ((uint32_t)(backend::TargetBufferFlags::DEPTH) & (uint32_t)result->targets)
            {
                result->gl.depth = static_cast<GLTexture*>(depth.m_tex);
                framebufferTexture(depth, result, GL_DEPTH_ATTACHMENT);
            }

            if ((uint32_t)(backend::TargetBufferFlags::STENCIL) & (uint32_t)result->targets)
            {
                result->gl.stencil = static_cast<GLTexture*>(stencil.m_tex);
                framebufferTexture(stencil, result, GL_STENCIL_ATTACHMENT);
            }
            
            return result;
        }
        void OpenGLDriver::clearWithRasterPipe(TargetBufferFlags clearFlags, math::Vector4 const& linearColor, GLfloat depth, GLint stencil) noexcept
        {
            glEnable(GL_DEPTH_TEST);
            if ((uint32_t)clearFlags & (uint32_t)TargetBufferFlags::COLOR0)
            {
                glClearBufferfv(GL_COLOR, 0, linearColor.v);
            }
            if ((uint32_t)clearFlags & (uint32_t)TargetBufferFlags::DEPTH)
            {
                glClearBufferfv(GL_DEPTH, 0, &depth);
            }
            CHECK_GL_ERROR();
        }
        void OpenGLDriver::beginRenderPass(backend::HwRenderTarget* rth, const backend::RenderPassParams& params)
        {
            GLRenderTarget* rt = static_cast<GLRenderTarget*>(rth);
            
            glBindFramebuffer(GL_FRAMEBUFFER, rt->gl.fbo);
            const TargetBufferFlags clearFlags = TargetBufferFlags((uint32_t)params.flags.clear & (uint32_t)rt->targets);
            if ((uint32_t)clearFlags > 0) {
                clearWithRasterPipe(clearFlags, params.clearColor, params.clearDepth, params.clearStencil);
            }
            if (params.viewport.getWidth() > 0 && params.viewport.getHeight() > 0)
            {
                glViewport(params.viewport.getLeft(),params.viewport.getBottom(),
                    params.viewport.getWidth(),params.viewport.getHeight());
            }
        }

        void OpenGLDriver::endRenderPass(int)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            CHECK_GL_ERROR();
        }
        void OpenGLDriver::update2DImage(
            backend::HwTexture* th,
            uint32_t level,
            uint32_t xoffset,
            uint32_t yoffset,
            uint32_t width,
            uint32_t height,
            void* data,
            size_t dataSize,
            backend::PixelDataFormat format,
            backend::PixelDataType type)
        {
            GLTexture* tex = static_cast<GLTexture *>(th);

            GLenum glformat = OpenGLUtils::getFormat(format);
            GLenum gltype = OpenGLUtils::getType(type);

            switch (tex->target)
            {
                case SamplerType::SAMPLER_2D:
                    glBindTexture(tex->gl.target, tex->gl.id);
                    glTexSubImage2D(tex->gl.target, GLint(level),
                        GLint(xoffset), GLint(yoffset),
                        width, height, glformat, gltype, data);
                    break;
            }
        }
        void OpenGLDriver::bindSamplers(uint32_t index, const backend::HwTexture* th)
        {
            const GLTexture* tex = static_cast<const GLTexture*>(th);
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(tex->gl.target, tex->gl.id);
            CHECK_GL_ERROR();
        }
        backend::HwSamplerGroup* OpenGLDriver::createSamplerGroup()
        {
            GLSamplerGroup* result = new GLSamplerGroup();
            return result;
        }
        bool OpenGLDriver::bindSamplerLocations(backend::HwProgram* program,
            std::vector<std::string> samplerNames)
        {
            // https://www.khronos.org/opengl/wiki/Example/Texture_Shader_Binding
            GLProgram* gl_program = static_cast<GLProgram*>(program);
            
            glUseProgram(gl_program->gl.program);
            for (size_t i = 0; i < samplerNames.size(); i++)
            {
                std::string name = samplerNames.at(i);
                GLint pos = glGetUniformLocation(gl_program->gl.program, name.c_str());
                if (!(pos < 0))
                    glUniform1i(pos, (GLint)i);
                else
                    return false;
            }
            glUseProgram(0);
            CHECK_GL_ERROR();
            return true;
        }
        // ------------------------------------Destroy APIS----------------------------------------
        void OpenGLDriver::DestroyRenderPrimitive(backend::HwRenderPrimitive* rp)
        {
            if (rp)
            {
                GLRenderPrimitive* glrp = static_cast<GLRenderPrimitive*>(rp);
                glDeleteVertexArrays(1, &(glrp->gl.vao));
                CHECK_GL_ERROR();
                delete glrp;
            }
        }
        void OpenGLDriver::DestroyIndexBuffer(backend::HwIndexBuffer* ib)
        {
            if (ib)
            {
                GLIndexBuffer* glib = static_cast<GLIndexBuffer*>(ib);
                glDeleteBuffers(1, &(glib->gl.buffer));
                CHECK_GL_ERROR();
                delete glib;
            }
        }
        void OpenGLDriver::DestroyVertexBuffer(backend::HwVertexBuffer* vb)
        {
            if (vb)
            {
                GLVertexBuffer* glvb = static_cast<GLVertexBuffer*>(vb);
                // hold only reference to glbufferobject, no gl stuff created
                delete glvb;
            }
        }
        void OpenGLDriver::DestroyBufferObject(backend::HwBufferObject* bo)
        {
            if (bo)
            {
                GLBufferObject* glbo = static_cast<GLBufferObject*>(bo);
                glDeleteBuffers(1, &(glbo->gl.id));
                CHECK_GL_ERROR();
                delete glbo;
            }
        }
        void OpenGLDriver::DestroySamplerGroup(backend::HwSamplerGroup* sg)
        {
            if (sg)
            {
                // the GLTexture it referenced does not need to be delete. the called would be responsible to release
                GLSamplerGroup* glsg = static_cast<GLSamplerGroup*>(sg);
                delete glsg;
            }
        }
        void OpenGLDriver::DestroyProgram(backend::HwProgram* pm)
        {
            if (pm)
            {
                GLProgram * glpm = static_cast<GLProgram *>(pm);

                glDetachShader(glpm->gl.program, glpm->gl.shaders[(uint32_t)backend::Shader::VERTEX]);
                glDetachShader(glpm->gl.program, glpm->gl.shaders[(uint32_t)backend::Shader::FRAGMENT]);
            
                glDeleteShader(glpm->gl.shaders[(uint32_t)backend::Shader::VERTEX]);
                glDeleteShader(glpm->gl.shaders[(uint32_t)backend::Shader::FRAGMENT]);
                glDeleteProgram(glpm->gl.program);
                CHECK_GL_ERROR();

                delete glpm;
            }
            
        }
        void OpenGLDriver::DestroyTexture(backend::HwTexture* tex)
        {
            if (tex)
            {
                GLTexture* gltex = static_cast<GLTexture*>(tex);
                glDeleteTextures(1, &(gltex->gl.id));
                CHECK_GL_ERROR();

                delete gltex;
            }
        }
        void OpenGLDriver::DestroyRenderTarget(backend::HwRenderTarget* mrt)
        {
            if (mrt)
            {
                // the GLTexture it referenced does not need to be delete. the called would be responsible to release
                GLRenderTarget* glmrt = static_cast<GLRenderTarget*>(mrt);
                // pass default frame buffer
                if (glmrt->gl.fbo != 0)
                {
                    glDeleteFramebuffers(1, &(glmrt->gl.fbo));
                    CHECK_GL_ERROR();
                }
                delete glmrt;
            }
        }
        void OpenGLDriver::DestroySwapChain(backend::HwSwapChain* sc)
        {
            if (sc)
            {
                m_platfrom->DestroySwapChain(sc->swapchain);
                delete sc;
            }
        }
    }
}
