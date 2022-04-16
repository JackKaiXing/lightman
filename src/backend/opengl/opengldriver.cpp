
#include "backend/opengl/opengldriver.h"
#include "backend/opengl/openglutils.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace lightman
{
    namespace backend
    {
        // TEST PROGRAM
        GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
        {

            // Create the shaders
            GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

            // Read the Vertex Shader code from the file
            std::string VertexShaderCode;
            std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
            if(VertexShaderStream.is_open()){
                std::string Line = "";
                while(getline(VertexShaderStream, Line))
                    VertexShaderCode += "\n" + Line;
                VertexShaderStream.close();
            }else{
                printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
                getchar();
                return 0;
            }

            // Read the Fragment Shader code from the file
            std::string FragmentShaderCode;
            std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
            if(FragmentShaderStream.is_open()){
                std::string Line = "";
                while(getline(FragmentShaderStream, Line))
                    FragmentShaderCode += "\n" + Line;
                FragmentShaderStream.close();
            }

            GLint Result = GL_FALSE;
            int InfoLogLength;


            // Compile Vertex Shader
            printf("Compiling shader : %s\n", vertex_file_path);
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
            printf("Compiling shader : %s\n", fragment_file_path);
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
            GLuint ProgramID = glCreateProgram();
            glAttachShader(ProgramID, VertexShaderID);
            glAttachShader(ProgramID, FragmentShaderID);
            glLinkProgram(ProgramID);

            // Check the program
            glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
            glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf("%s\n", &ProgramErrorMessage[0]);
            }
            
            glDetachShader(ProgramID, VertexShaderID);
            glDetachShader(ProgramID, FragmentShaderID);
            
            glDeleteShader(VertexShaderID);
            glDeleteShader(FragmentShaderID);

            return ProgramID;
        }
        GLuint g_program = 0;
        // ----------------------------------------------------------------------------
        backend::Driver* OpengGLDriver::create(backend::OpenGLPlatform* platform, void* sharedGLContext) noexcept
        {
            g_program = LoadShaders("/Users/XK/Desktop/Projects/lightman/src/backend/opengl/shader.vert",
            "/Users/XK/Desktop/Projects/lightman/src/backend/opengl/shader.frag");
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
            CHECK_GL_ERROR();
            return result;
        }
        HwVertexBuffer* OpengGLDriver::createVertexBuffer(
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
        HwBufferObject* OpengGLDriver::createBufferObject(
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
        void OpengGLDriver::updateBufferObject(backend::HwBufferObject* boh, void* data, size_t dataSize, uint32_t byteOffset)
        {
            GLBufferObject* bo = static_cast<GLBufferObject*>(boh);

            assert(dataSize + byteOffset <= bo->byteCount);

            if (bo->gl.binding == GL_UNIFORM_BUFFER)
            {
                // TODO
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
        void OpengGLDriver::setVertexBufferObject(backend::HwVertexBuffer* vbh,
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
        HwIndexBuffer* OpengGLDriver::createIndexBuffer(backend::ElementType elementType,
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
        void OpengGLDriver::updateIndexBuffer(backend::HwIndexBuffer* ibh,
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
        void OpengGLDriver::setRenderPrimitiveBuffer(backend::HwRenderPrimitive* rph,
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
            }
            CHECK_GL_ERROR();
        }

        void OpengGLDriver::draw(backend::HwRenderPrimitive* rph)
        {
            GLRenderPrimitive* rp = static_cast<GLRenderPrimitive *>(rph);

            glUseProgram(g_program);

            glBindVertexArray(rp->gl.vao);
            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawRangeElements.xhtml
            glDrawRangeElements(GLenum(rp->type), rp->minIndex, rp->maxIndex, rp->count,
                rp->gl.indicesType, reinterpret_cast<const void*>(rp->offset));
            CHECK_GL_ERROR();
        }
    }
}
