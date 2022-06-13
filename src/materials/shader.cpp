
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
                case backend::UniformType::INT:
                    return "int";
                case backend::UniformType::BOOL:
                    return "bool";
                case backend::UniformType::FLOAT:
                    return "float";
                case backend::UniformType::FLOAT3:
                    return "vec3";
                case backend::UniformType::FLOAT4:
                    return "vec4";
                    
                default:
                    return "";;
            }
        }
        std::string UniformTypeToShaderString(backend::UniformType type)
        {
            return UniformTypeToShaderString_OPENGL(type);
        }
        std::string CreateBlockInfo(const std::vector<UniformDefine>& uDefine,
            const std::vector<SamplerDefine>& sDefine,
            const std::string& UnoformBlockName)
        {
            // uniform block
            std::string result = "layout (std140) uniform " + UnoformBlockName + "\n \
            {\n ";
            for (auto iter = uDefine.begin(); iter != uDefine.end(); iter++)
            {
                if (iter->size > 1)
                    result += UniformTypeToShaderString(iter->type) + " " + iter->name + "[" + std::to_string(iter->size) + "]" + "; \n";
                else
                    result += UniformTypeToShaderString(iter->type) + " " + iter->name + "; \n";
            }
            result += "}; \n ";
            // texture block
            int i = 0;
            for (auto iter = sDefine.begin(); iter != sDefine.end(); iter++)
            {
                // result += "layout(binding = " + std::to_string(i) + ") "; // required version 420
                result += "uniform ";
                switch (iter->type)
                {
                case backend::SamplerType::SAMPLER_2D:
                    result += "sampler2D ";
                    break;
                
                default:
                    break;
                }
                result += iter->name + ";\n";
            }
            
            return result;
        }
        void GetSharedBlockInfo(std::vector<UniformDefine>& uDefines)
        {
            uDefines.push_back({SharedUniformName_PVMMatrix, backend::UniformType::MAT4, 1, backend::Precision::DEFAULT});
            uDefines.push_back({SharedUniformName_InverseMMatrix, backend::UniformType::MAT4, 1, backend::Precision::DEFAULT});
            uDefines.push_back({SharedUniformName_HasNormal, backend::UniformType::INT, 1, backend::Precision::DEFAULT});
            uDefines.push_back({SharedUniformName_HasUV0, backend::UniformType::INT, 1, backend::Precision::DEFAULT});
            uDefines.push_back({SharedUniformName_HasUV1, backend::UniformType::INT, 1, backend::Precision::DEFAULT});
            uDefines.push_back({SharedUniformName_CameraPos, backend::UniformType::FLOAT3, 1, backend::Precision::DEFAULT});
        }
        const std::string GetFragmentShaderHead()
        {
            std::string result;

            result = "#version 330 core \n \
            layout (location = 0) out vec4 color; \n";
            
            return result;
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

            result = "struct DisneyBRDFMaterialParas \n \
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
        void GetBlenderBlockInfo(std::vector<UniformDefine>& uDefines)
        {
            uDefines.push_back({"world_light_direction", backend::UniformType::FLOAT4, 4, backend::Precision::DEFAULT});
            uDefines.push_back({"world_light_specular_color", backend::UniformType::FLOAT4, 4, backend::Precision::DEFAULT});
            uDefines.push_back({"world_light_diffuse_color_wrap", backend::UniformType::FLOAT4, 4, backend::Precision::DEFAULT});
            uDefines.push_back({"world_light_ambient_color", backend::UniformType::FLOAT4, 1, backend::Precision::DEFAULT});
        }
        const std::string GetBlenderVertexShader()
        {
            std::string result = "out vec3 vNormal; \n \
                out vec3 vViewDir; \n \
                out vec2 vUV0; \n \
                void main() \n \
                { \n \
                    vec4 transformedTangent = InverseMMatrix * vec4(tangent, 0.0); \n \
                    vNormal = normalize(transformedTangent.xyz); \n \
                    \n \
                    vViewDir = cameraPos - position; \n \
                    vViewDir = normalize(vViewDir); \n \
                    vUV0 = uv0; \n \
                    gl_Position = PVMMatrix * vec4(position, 1.0f); \n \
                }";
            return result;
        }
        const std::string GetBlenderFragmentShader(const std::string& updateMaterialString)
        {
            // This is heavily relied on blender workbench shading design
            // https://github.com/blender/blender/tree/master/source/blender/draw/engines/workbench

            // fast_rcp
            // https://github.com/michaldrobot/ShaderFastLibs/blob/master/ShaderFastMathLib.h

            // wrap_shading
            // https://www.iro.umontreal.ca/~derek/files/jgt_wrap.pdf
            // https://developer.nvidia.com/gpugems/gpugems/part-iii-materials/chapter-16-real-time-approximations-subsurface-scattering
            // https://www.cnblogs.com/cpxnet/p/6075353.html

            // Energy Conservation or normalization factor for Blinn Phong Specular and Approximation   
            // http://www.thetenthplanet.de/archives/255
            // http://www.farbrausch.de/~fg/stuff/phong.pdf

            // Energy COnservation for Blinn Phong Diffuse
            // https://www.rorydriscoll.com/2009/01/25/energy-conservation-in-games/

            std::string result;

            result = "in vec3 vNormal; \n \
                in vec3 vViewDir; \n \
                in vec2 vUV0; \n \
                struct MaterialParas \n \
                { \n \
                    vec3 baseColor; \n \
                    float metallic; \n \
                    float roughness; \n \
                } ; \n \
                float Luminance(vec3 c) \n \
                { \n \
                    return 0.212671 * c.x + 0.715160 * c.y + 0.072169 * c.z; \n \
                } \n \
                void InitMaterialParameters(inout MaterialParas paras) \n \
                { \n \
                    paras.baseColor = vec3(1.0f); \n \
                    paras.metallic = 0.0f; \n \
                    paras.roughness = 0.5f; \n \
                } \n ";

            result += "void UpdateUserMaterialParameters(inout MaterialParas paras) \n \
                    { \n";
            if (updateMaterialString.size() > 0)
                result += updateMaterialString;
            result += "} \n";
            
            result += "vec4 fast_rcp(vec4 v) \n \
                { \n \
                    return intBitsToFloat(0x7eef370b - floatBitsToInt(v)); \n \
                } \n \
                \n \
                void prep_specular(vec3 L, vec3 I, vec3 N, vec3 R, out float NL, out float wrapped_NL, out float spec_angle)\n \
                {\n \
                    wrapped_NL = dot(L, R);\n \
                    vec3 half_dir = normalize(L + I);\n \
                    spec_angle = clamp(dot(half_dir, N), 0.0, 1.0);\n \
                    NL = clamp(dot(L, N), 0.0, 1.0);\n \
                }\n \
                /* Normalized Blinn shading */\n \
                vec4 blinn_specular(vec4 shininess, vec4 spec_angle, vec4 NL)\n \
                {\n \
                    /* Pi is already divided in the light power. normalization_factor = (shininess + 8.0) / (8.0 * M_PI) */ \n \
                    vec4 normalization_factor = shininess * 0.125 + 1.0; \n \
                    vec4 spec_light = pow(spec_angle, shininess) * NL * normalization_factor; \n \
                    return spec_light;\n \
                }\n \
                /* NL need to be unclamped. w in [0..1] range. */\n \
                vec4 wrapped_lighting(vec4 NL, vec4 w)\n \
                {\n \
                    vec4 w_1 = w + 1.0;\n \
                    vec4 denom = fast_rcp(w_1 * w_1);\n \
                    return clamp((NL + w) * denom, 0.0, 1.0);\n \
                }\n \
                vec3 brdf_approx(vec3 spec_color, float roughness, float NV)\n \
                {\n \
                    /* Very rough own approx. We don't need it to be correct, just fast. Just simulate fresnel effect with roughness attenuation. */ \n \
                    float fresnel = exp2(-8.35 * NV) * (1.0 - roughness);\n \
                    return mix(spec_color, vec3(1.0), fresnel);\n \
                }\n \
                vec3 get_world_lighting(vec3 base_color, float roughness, float metallic, vec3 N, vec3 I)\n \
                {\n \
                    vec3 specular_color, diffuse_color;\n \
                    diffuse_color = mix(base_color, vec3(0.0), metallic);\n \
                    specular_color = mix(vec3(0.05), base_color, metallic);\n \
                    vec3 specular_light = world_light_ambient_color.rgb;\n \
                    vec3 diffuse_light = world_light_ambient_color.rgb;\n \
                    vec4 wrap = vec4(world_light_diffuse_color_wrap[0].a, \n \
                                    world_light_diffuse_color_wrap[1].a, \n \
                                    world_light_diffuse_color_wrap[2].a, \n \
                                    world_light_diffuse_color_wrap[3].a);\n \
                    {\n \
                        /* Prepare Specular computation. Eval 4 lights at once. */\n \
                        vec3 R = -reflect(I, N);\n \
                        vec4 spec_angle, spec_NL, wrap_NL;\n \
                        prep_specular(world_light_direction[0].xyz, I, N, R, spec_NL.x, wrap_NL.x, spec_angle.x);\n \
                        prep_specular(world_light_direction[1].xyz, I, N, R, spec_NL.y, wrap_NL.y, spec_angle.y);\n \
                        prep_specular(world_light_direction[2].xyz, I, N, R, spec_NL.z, wrap_NL.z, spec_angle.z);\n \
                        prep_specular(world_light_direction[3].xyz, I, N, R, spec_NL.w, wrap_NL.w, spec_angle.w);\n \
                        \n \
                        vec4 gloss = vec4(1.0 - roughness);\n \
                        /* Reduce gloss for smooth light. (simulate bigger light) */\n \
                        gloss *= 1.0 - wrap;\n \
                        vec4 shininess = exp2(10.0 * gloss + 1.0);\n \
                        \n \
                        vec4 spec_light = blinn_specular(shininess, spec_angle, spec_NL);\n \
                        \n \
                        /* Simulate Env. light. */ \n \
                        vec4 w = mix(wrap, vec4(1.0), roughness); \n \
                        vec4 spec_env = wrapped_lighting(wrap_NL, w); \n \
                        \n \
                        spec_light = mix(spec_light, spec_env, wrap * wrap);\n \
                        \n \
                        /* Multiply result by lights specular colors. */\n \
                        specular_light += spec_light.x * world_light_specular_color[0].rgb;\n \
                        specular_light += spec_light.y * world_light_specular_color[1].rgb;\n \
                        specular_light += spec_light.z * world_light_specular_color[2].rgb;\n \
                        specular_light += spec_light.w * world_light_specular_color[3].rgb;\n \
                        \n \
                        float NV = clamp(dot(N, I), 0.0, 1.0);\n \
                        specular_color = brdf_approx(specular_color, roughness, NV);\n \
                    }\n \
                    specular_light *= specular_color;\n \
                    \n \
                    /* Prepare diffuse computation. Eval 4 lights at once. */\n \
                    vec4 diff_NL;\n \
                    diff_NL.x = dot(world_light_direction[0].xyz, N);\n \
                    diff_NL.y = dot(world_light_direction[1].xyz, N);\n \
                    diff_NL.z = dot(world_light_direction[2].xyz, N);\n \
                    diff_NL.w = dot(world_light_direction[3].xyz, N);\n \
                    \n \
                    vec4 diff_light = wrapped_lighting(diff_NL, wrap);\n \
                    \n \
                    /* Multiply result by lights diffuse colors. */\n \
                    diffuse_light += diff_light.x * world_light_diffuse_color_wrap[0].rgb;\n \
                    diffuse_light += diff_light.y * world_light_diffuse_color_wrap[1].rgb;\n \
                    diffuse_light += diff_light.z * world_light_diffuse_color_wrap[2].rgb;\n \
                    diffuse_light += diff_light.w * world_light_diffuse_color_wrap[3].rgb;\n \
                    \n \
                    /* Energy conservation with colored specular look strange. Limit this strangeness by using mono-chromatic specular intensity. */\n \
                    float spec_energy = dot(specular_color, vec3(0.33333));\n \
                    \n \
                    diffuse_light *= diffuse_color * (1.0 - spec_energy);\n \
                    \n \
                    return diffuse_light + specular_light;\n \
                }\n \
                void main() \n \
                { \n \
                    MaterialParas paras;  \n \
                    InitMaterialParameters(paras);  \n \
                    UpdateUserMaterialParameters(paras);  \n \
                    \n \
                    color.rgb = get_world_lighting(paras.baseColor, paras.roughness, paras.metallic, vNormal, vViewDir);  \n \
                    color.a = 1.0;  \n \
                }";

            return result;
        }
    } // namespace ShaderString    
} // namespace lightman
