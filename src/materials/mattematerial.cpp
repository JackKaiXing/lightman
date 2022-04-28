
#include "materials/mattematrial.h"

#include <sstream>

namespace lightman
{
    MatteMaterial::MatteMaterial(const std::string& name) 
        : Material(name)
    {
        // TODO read uniform config from outside
        std::vector<UniformDefine> uDefines;
        uDefines.push_back({"PVMMatrix", backend::UniformType::MAT4, 1, backend::Precision::DEFAULT});
        uDefines.push_back({"uTestColor", backend::UniformType::FLOAT, 1, backend::Precision::DEFAULT});

        InitUniformBlockInfo(uDefines);
    }
    MatteMaterial::~MatteMaterial()
    {

    }
    std::string MatteMaterial::CreateVertexShaderString(uint32_t index)
    {
        // TODO Remove as outside config
        std::stringstream ss;
        ss << "#version 330 core" << "\n";

        if (index & backend::VertexAttribute::TANGENTS)
            ss << "#define " << "HAS_ATTRIBUTE_TANGENTS" << "\n";
        
        if (index & backend::VertexAttribute::UV0)
            ss << "#define " << "HAS_ATTRIBUTE_UV0" << "\n";

        if (index & backend::VertexAttribute::UV1)
            ss << "#define " << "HAS_ATTRIBUTE_UV1" << "\n";

        static const std::string vertexShader = "layout(location = 0) in vec3 position; \n \
            #if defined(HAS_ATTRIBUTE_TANGENTS) \n \
                layout(location = 1) in vec3 tangent; \n \
            #endif \n \
            \n \
            #if defined(HAS_ATTRIBUTE_UV0) \n \
                layout(location = 2) in vec2 uv0; \n \
                // uniform sampler2d texture0; // TODO \n \
            #endif \n \
            \n \
            #if defined(HAS_ATTRIBUTE_UV1) \n \
                layout(location = 3) in vec2 uv1; \n \
                // uniform sampler2d texture1; \n \
            #endif \n \
            \n \
            layout (std140) uniform targetUniform \n \
            {\n \
                mat4 PVMMatrix; \n \
                float uTestColor; \n \
            }; \n \
            out float lightIntensity; \n \
            void main() \n \
            { \n \
                lightIntensity = 1.0; \n \
                #if defined(HAS_ATTRIBUTE_TANGENTS) \n \
                    lightIntensity *= dot(vec3(0.577), tangent); \n \
                #endif \n \
                mat4 localMatrix; \n \
                localMatrix[0] = vec4(0,        0,          -1.00400794,    -1); \n \
                localMatrix[1] = vec4(0,        2.1875,     0,              0); \n \
                localMatrix[2] = vec4(1.75,     0,          0,              0); \n \
                localMatrix[3] = vec4(0,        0,          9.63927745,    10); \n \
                gl_Position = PVMMatrix * vec4(position, 1.0f); \n \
            }";
        ss << vertexShader;

        return ss.str();
    }
    std::string MatteMaterial::CreateFragmentShaderString(uint32_t index)
    {
        std::stringstream ss;
        ss << "#version 330 core" << "\n";

        if (index & backend::VertexAttribute::TANGENTS)
            ss << "#define " << "HAS_ATTRIBUTE_TANGENTS" << "\n";
        
        if (index & backend::VertexAttribute::UV0)
            ss << "#define " << "HAS_ATTRIBUTE_UV0" << "\n";

        if (index & backend::VertexAttribute::UV1)
            ss << "#define " << "HAS_ATTRIBUTE_UV1" << "\n";

        static const std::string fragmengShader = "out vec4 color; \n \
        in float lightIntensity; \n \
        void main() \n \
        { \n \
            color.rgb =	vec3(lightIntensity); \n \
            color.a = 1.0; \n \
        }";
        ss << fragmengShader;

        return ss.str();
    }
} // namespace lightman
