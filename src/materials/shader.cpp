
#include "materials/shader.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    namespace ShaderString
    {
        std::string GetVertexAttribute()
        {
            // opengl
            std::string result = 
                "#version 330 core \n \
                layout(location = 0) in vec3 position; \n \
                layout(location = 1) in vec3 tangent; \n \
                layout(location = 2) in vec2 uv0; \n \
                layout(location = 3) in vec2 uv1; \n";

            // TODO metal, vulkan, opengl es
            
            return result;
        }

        std::string UniformTypeToShaderString_OPENGL(backend::UniformType type)
        {
            switch (type) {
                case backend::UniformType::MAT4:
                    return "mat4";
                    break;
                case backend::UniformType::INT:
                    return "int";
                    break;
                case backend::UniformType::BOOL:
                    return "bool";
                    break;
                    
                default:
                    break;
            }
        }
        std::string UniformTypeToShaderString(backend::UniformType type)
        {
            return UniformTypeToShaderString_OPENGL(type);
        }
        backend::UniformBlockInfo GetUniformBufferInfo()
        {
            backend::UniformBlockInfo info;
            info.at(0) = "targetUniform";
            return info;
        }
        std::string CreateBlockInfo(const std::vector<UniformDefine>& uDefine,
            const std::string& UnoformBlockName)
        {
            std::string result = "layout (std140) uniform " + UnoformBlockName + "\n \
            {\n ";
            for (auto iter = uDefine.begin(); iter != uDefine.end(); iter++)
            {
                result += UniformTypeToShaderString(iter->type) + " " + iter->name + "; \n";
            }
            result += "}; \n ";
            return result;
        }
    } // namespace ShaderString    
} // namespace lightman
