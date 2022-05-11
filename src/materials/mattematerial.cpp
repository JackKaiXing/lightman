
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
        uDefines.push_back({"InverseMMatrix", backend::UniformType::MAT4, 1, backend::Precision::DEFAULT});
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
                mat4 InverseMMatrix; \n \
                float uTestColor; \n \
            }; \n \
            out vec3 Normal; \n \
            void main() \n \
            { \n \
                #if defined(HAS_ATTRIBUTE_TANGENTS) \n \
                    vec4 transformedTangent = InverseMMatrix * vec4(tangent, 0.0); \n \
                    Normal = normalize(transformedTangent.xyz); \n \
                #endif \n \
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

        static const std::string fragmengShader = "#define RECIPROCAL_PI 0.3183098861837907 \n \
        layout (location = 0) out vec4 color; \n \
        in vec3 Normal; \n \
        vec3 LinearTosRGB( in vec3 value ) { \n \
            return vec3( mix( pow( value.rgb, vec3( 0.41666 ) ) * 1.055 - vec3( 0.055 ), value.rgb * 12.92, vec3( lessThanEqual( value.rgb, vec3( 0.0031308 ) ) ) )); \n \
        } \n \
        void main() \n \
        { \n \
            vec3 ambientLightColor = vec3(0.6283, 0.6283, 0.6283); \n \
            vec3 LightColor = vec3(3.1416, 3.1416, 3.1416); \n \
            vec3 lightDirection = vec3(0.6767, 0.2120, 0.7051); \n \
            color.rgb =	(dot(Normal,lightDirection) / 2.0 + 0.5) * LightColor + ambientLightColor; \n \
            color.rgb *= RECIPROCAL_PI; \n \
            color.rgb = LinearTosRGB(color.rgb); \n \
            color.a = 1.0; \n \
        }";
        ss << fragmengShader;

        return ss.str();
    }
} // namespace lightman
