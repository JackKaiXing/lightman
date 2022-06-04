
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
                case backend::UniformType::FLOAT3:
                    return "vec3";
                    break;
                    
                default:
                    break;
            }
        }
        std::string UniformTypeToShaderString(backend::UniformType type)
        {
            return UniformTypeToShaderString_OPENGL(type);
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
        void GetSharedBlockInfo(std::vector<UniformDefine>& uDefines)
        {
            uDefines.push_back({"PVMMatrix", backend::UniformType::MAT4, 1, backend::Precision::DEFAULT});
            uDefines.push_back({"InverseMMatrix", backend::UniformType::MAT4, 1, backend::Precision::DEFAULT});
            uDefines.push_back({"HasNormal", backend::UniformType::INT, 1, backend::Precision::DEFAULT});
            uDefines.push_back({"HasUV0", backend::UniformType::INT, 1, backend::Precision::DEFAULT});
            uDefines.push_back({"HasUV1", backend::UniformType::INT, 1, backend::Precision::DEFAULT});
            uDefines.push_back({"cameraPos", backend::UniformType::FLOAT3, 1, backend::Precision::DEFAULT});
        }
        const std::string GetDisneyVertexShader()
        {
            std::string result = "out vec3 vNormal; \n \
                out vec3 vTangent; \n \
                out vec3 vBitangent; \n \
                out vec3 vViewDir; \n \
                void main() \n \
                { \n \
                    vNormal = vec3(0.0, 0.0, 1.0); \n \
                    if(HasNormal > 0) \n \
                    { \n \
                        vec4 transformedTangent = InverseMMatrix * vec4(tangent, 0.0); \n \
                        vNormal = normalize(transformedTangent.xyz); \n \
                    } \n \
                    \n \
                    vViewDir = cameraPos - position; \n \
                    vViewDir = normalize(vViewDir); \n \
                    vec3 up = abs(vNormal.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0); \n \
                    vTangent = normalize(cross(up, vNormal)); \n \
                    vBitangent = cross(vNormal, vTangent); \n \
                    gl_Position = PVMMatrix * vec4(position, 1.0f); \n \
                }";
            return result;
        }
        const std::string GetDisneyFragmentShader(const std::string& updateMaterialString)
        {
            std::string result;

            result = "#version 330 core \n \
                layout (location = 0) out vec4 color; \n \
                struct DisneyBRDFMaterialParas \n \
                { \n \
                    vec3 baseColor; \n \
                    float metallic; \n \
                    float subsurface ; \n \
                    float specular; \n \
                    float roughness; \n \
                    float specularTint; \n \
                    float anisotropic; \n \
                    float sheen; \n \
                    float sheenTint; \n \
                    float clearcoat; \n \
                    float clearcoatGloss; \n \
                } ; \n \
                void InitMaterialParameters(inout DisneyBRDFMaterialParas paras) \n \
                { \n \
                    paras.baseColor = vec3(1.0f); \n \
                    paras.metallic = 0.0f; \n \
                    paras.subsurface = 0.0f; \n \
                    paras.specular = 0.5f; \n \
                    paras.roughness = 0.5f; \n \
                    paras.specularTint = 0.0f; \n \
                    paras.anisotropic = 0.0f; \n \
                    paras.sheen = 0.0f; \n \
                    paras.sheenTint = 0.5f; \n \
                    paras.clearcoat = 0.0f; \n \
                    paras.clearcoatGloss = 1.0f; \n \
                } \n ";

            if (updateMaterialString.size() > 0)
                result += updateMaterialString;
            else
                result += "void UpdateUserMaterialParameters(inout DisneyBRDFMaterialParas paras) \n \
                    { \n \
                    }";
            
            result += "in vec3 vNormal; \n \
                in vec3 vTangent; \n \
                in vec3 vBitangent; \n \
                in vec3 vViewDir; \n \
                const float PI = 3.14159265358979323846; \n \
                float sqr(float x) { return x*x; }  \n \
                float SchlickFresnel(float u)  \n \
                {  \n \
                    float m = clamp(1-u, 0, 1);  \n \
                    float m2 = m*m;  \n \
                    return m2*m2*m; // pow(m,5)  \n \
                }  \n \
                float GTR1(float NdotH, float a) \n \
                {  \n \
                    if (a >= 1) return 1/PI;  \n \
                    float a2 = a*a;  \n \
                    float t = 1 + (a2-1)*NdotH*NdotH;  \n \
                    return (a2-1) / (PI*log(a2)*t);  \n \
                }  \n \
                float GTR2(float NdotH, float a)  \n \
                {  \n \
                    float a2 = a*a;  \n \
                    float t = 1 + (a2-1)*NdotH*NdotH;  \n \
                    return a2 / (PI * t*t);  \n \
                }  \n \
                float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)  \n \
                {  \n \
                    return 1 / (PI * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));  \n \
                }  \n \
                float smithG_GGX(float NdotV, float alphaG)\n \
                {\n \
                    float a = alphaG*alphaG;\n \
                    float b = NdotV*NdotV;\n \
                    return 1 / (NdotV + sqrt(a + b - a*b));\n \
                }\n \
                float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay) \n \
                {\n \
                    return 1 / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));\n \
                }\n \
                vec3 mon2lin(vec3 x)\n \
                {\n \
                    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2)); \n \
                }\n \
                vec3 DisneyBRDF( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y, DisneyBRDFMaterialParas paras)\n \
                {\n \
                    float NdotL = dot(N,L);\n \
                    float NdotV = dot(N,V);\n \
                    if (NdotL < 0 || NdotV < 0) return vec3(0);\n \
                    \n \
                    vec3 H = normalize(L+V);\n \
                    float NdotH = dot(N,H);\n \
                    float LdotH = dot(L,H);\n \
                    \n \
                    vec3 Cdlin = mon2lin(paras.baseColor);\n \
                    float Cdlum = .3*Cdlin[0] + .6*Cdlin[1]  + .1*Cdlin[2]; // luminance approx.\n \
                    \n \
                    vec3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : vec3(1); // normalize lum. to isolate hue+sat \n \
                    vec3 Cspec0 = mix(paras.specular*.08*mix(vec3(1), Ctint, paras.specularTint), Cdlin, paras.metallic); \n \
                    vec3 Csheen = mix(vec3(1), Ctint, paras.sheenTint); \n \
                    \n \
                    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing \n \
                    // and mix in diffuse retro-reflection based on roughness \n \
                    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV); \n \
                    float Fd90 = 0.5 + 2 * LdotH*LdotH * paras.roughness; \n \
                    float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV); \n \
                    \n \
                    // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf \n \
                    // 1.25 scale is used to (roughly) preserve albedo \n \
                    // Fss90 used to flatten retroreflection based on roughness \n \
                    float Fss90 = LdotH*LdotH*paras.roughness; \n \
                    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV); \n \
                    float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5); \n \
                    \n \
                    // specular \n \
                    float aspect = sqrt(1-paras.anisotropic*.9); \n \
                    float ax = max(.001, sqr(paras.roughness)/aspect); \n \
                    float ay = max(.001, sqr(paras.roughness)*aspect); \n \
                    float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay); \n \
                    float FH = SchlickFresnel(LdotH); \n \
                    vec3 Fs = mix(Cspec0, vec3(1), FH); \n \
                    float Gs; \n \
                    Gs  = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay); \n \
                    Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay); \n \
                    \n \
                    // sheen \n \
                    vec3 Fsheen = FH * paras.sheen * Csheen; \n \
                    \n \
                    // clearcoat (ior = 1.5 -> F0 = 0.04) \n \
                    float Dr = GTR1(NdotH, mix(.1,.001,paras.clearcoatGloss)); \n \
                    float Fr = mix(.04, 1.0, FH); \n \
                    float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25); \n \
                    \n \
                    return ((1/PI) * mix(Fd, ss, paras.subsurface)*Cdlin + Fsheen) \n \
                        * (1-paras.metallic) \n \
                        + Gs*Fs*Ds + .25*paras.clearcoat*Gr*Fr*Dr; \n \
                } \n \
                void main() \n \
                { \n \
                    DisneyBRDFMaterialParas paras;  \n \
                    InitMaterialParameters(paras);  \n \
                    UpdateUserMaterialParameters(paras);  \n \
                    \n \
                    vec3 lightDir = vec3(0.6767, 0.2120, 0.7051);\n \
                    color.rgb = DisneyBRDF(lightDir, vViewDir, vNormal, vTangent, vBitangent, paras);  \n \
                    color.a = 1.0;  \n \
                }";

            return result;
        }
    } // namespace ShaderString    
} // namespace lightman
