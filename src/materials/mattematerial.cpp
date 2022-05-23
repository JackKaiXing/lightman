
#include "materials/mattematrial.h"
#include "engine/engine.h"
#include "materials/shader.h"

#include <sstream>

namespace lightman
{
    MatteMaterial::MatteMaterial(const std::string& name) 
        : Material(name, nullptr, nullptr)
    {
    }
    MatteMaterial::MatteMaterial(const std::string& name, const Texture* bump, const Texture* emission, const Texture* kd) 
        : Material(name, bump, emission)
    {
    }
    MatteMaterial::~MatteMaterial()
    {
    }
    bool MatteMaterial::PrepareForRasterGPU()
    {
        if(m_isRasterGPUNeedUpdate)
            return true;

        std::vector<UniformDefine> uDefines;
        // get shared uniform defines
        GetSharedBlockInfo(uDefines);

        // add custom uniform defines
        // TO DO PARSE TEXTURE NODES TO GET UDEFINES
        
        // Init Block Infos
        InitUniformBlockInfo(uDefines);

        // update defaut materialInstance
        UpdateDefaultMaterialInstance();

        // TODO UPDATE MaterialInstance about uDefines

        // Update shaders
        backend::UniformBlockInfo bInfos = ShaderString::GetUniformBufferInfo();
        
        std::string vertexShaderString = ShaderString::GetVertexAttribute();
        vertexShaderString += ShaderString::CreateBlockInfo(uDefines, bInfos.at(0));
        vertexShaderString += "out vec3 Normal; \n \
            void main() \n \
            { \n \
                Normal = vec3(0.0,0.0,0.0); \n \
                if(HasNormal > 0) \n \
                { \n \
                    vec4 transformedTangent = InverseMMatrix * vec4(tangent, 0.0); \n \
                    Normal = normalize(transformedTangent.xyz); \n \
                } \n \
                gl_Position = PVMMatrix * vec4(position, 1.0f); \n \
            }";

        std::string fragmentShaderString = "#version 330 core \n \
        #define RECIPROCAL_PI 0.3183098861837907 \n";
        fragmentShaderString += ShaderString::CreateBlockInfo(uDefines, bInfos.at(0));
        fragmentShaderString += "layout (location = 0) out vec4 color; \n \
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

        // Update Program
        m_program = Engine::GetInstance()->GetDriver()->createProgram(
            vertexShaderString, fragmentShaderString, bInfos);

        m_isRasterGPUNeedUpdate = true;
        return true;
    }
} // namespace lightman
