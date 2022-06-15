
#include "materials/material.h"
#include "engine/engine.h"
#include "managers/materialmanager.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    Material::Material(const std::string& name, const Node* bump, const Node* emission)
    {
        m_name = name;
        m_bump = bump;
        m_emission = emission;
    }
    Material::~Material()
    {
        if (m_bump)
            delete m_bump;
        if (m_emission)
            delete m_emission;
        if (m_defaultMI)
            delete m_defaultMI;
    }
    MaterialInstance* Material::createMaterialInstance(const std::string& name)
    {
        IncreaseRef();
        
        MaterialInstance* result = new MaterialInstance(this, name);
        return result;
    }
    uint32_t Material::GetProgramIndexBySupportedVertexAttribute(bool hasTangent, bool hasUV0, bool hasUV1)
    {
        uint32_t index = 1; // only backend::VertexAttribute::POSITION;

        if (hasTangent)
            index += 0x01 << backend::VertexAttribute::TANGENTS;

        if (hasUV0)
            index += 0x01 << backend::VertexAttribute::UV0;
        
        if (hasUV1)
            index += 0x01 << backend::VertexAttribute::UV1;

        return index;
    }
    void Material::UpdateDefaultMaterialInstance()
    {
        if (m_defaultMI)
        {
            delete m_defaultMI;
        }
        // the default material instance is managed hy itself, not MI Manager, should be release by itself.
        m_defaultMI = new MaterialInstance(this, m_name + "_defaultMatInstance");
    }
    void Material::InitUniformBlockInfo(const std::vector<UniformDefine> uDefines)
    {
        // https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
        // https://www.khronos.org/opengl/wiki/Uniform_Buffer_Object
        m_uniformsInfoList.resize(uDefines.size());
        uint16_t offset = 0;
        int count = uDefines.size();
        for (size_t i = 0; i < count; i++)
        {
            // unit : sizeof(uint32_t)
            size_t alignment = Material::BaseAlignmentForType(uDefines[i].type);
            uint8_t stride = Material::StrideForType(uDefines[i].type);
            if (uDefines[i].size > 1) { 
                alignment = (alignment + 3) & ~3;
                stride = (stride + uint8_t(3)) & ~uint8_t(3);
            }
            size_t padding = (alignment - (offset % alignment)) % alignment;
            offset += padding;

            UniformInfo& info = m_uniformsInfoList[i];
            info = { uDefines[i].name, offset, stride, uDefines[i].type, uDefines[i].size, uDefines[i].precision};
            m_uniformInfoMap.insert({uDefines[i].name,i});

            offset += stride * uDefines[i].size;
        }
        
        m_uniformsSize = sizeof(uint32_t) * ((offset + 3) & ~3);
    }
    void Material::InitSamplerBlockInfo(const std::vector<SamplerDefine> sDefines)
    {
        int size = sDefines.size();

        m_samplerInfoMap.reserve(size);
        m_samplersInfoList.resize(size);

        int i = 0;
        for (auto iter = sDefines.begin(); iter != sDefines.end(); iter++)
        {
            SamplerInfo& info = m_samplersInfoList[i];
            info = { iter->name, iter->imgName, uint8_t(i), iter->type, iter->format, iter->multiSample, iter->precision };
            m_samplerInfoMap.insert({info.name, i});

            i++;
        }
    }
    bool Material::HasUniform(const std::string name)
    {
        return m_uniformInfoMap.find(name) != m_uniformInfoMap.end();
    }
    void Material::GetUniformOffsetAndStrideByName(const std::string name, uint32_t& offset, uint32_t& stride, uint32_t count)
    {
        uint32_t index = m_uniformInfoMap.find(name)->second;
        if (index >=0 && index < m_uniformsInfoList.size())
        {
            stride = sizeof(uint32_t) * m_uniformsInfoList[index].stride;
            offset = sizeof(uint32_t) * m_uniformsInfoList[index].offset;
            if (m_uniformsInfoList[index].size > count)
            {
                offset += count * sizeof(uint32_t) * StrideForType(m_uniformsInfoList[index].type);
            }
        }
    }
    void Material::PrepareForRasterGPUBase(const std::vector<const Node*>& customTextures, const std::string& UpdateUserMaterialParameters)
    {
        std::vector<UniformDefine> uDefines;
        std::vector<SamplerDefine> sDefines;
        // get shared uniform defines
        ShaderString::GetSharedBlockInfo(uDefines);
        // get blender workbench uniform defines
        ShaderString::GetBlenderBlockInfo(uDefines);
        // add custom uniform defines
        if (m_bump)
            m_bump->GetBlockInfo(uDefines,sDefines);
        if (m_emission)
            m_emission->GetBlockInfo(uDefines,sDefines);

        for (size_t i = 0; i < customTextures.size(); i++)
            customTextures.at(i)->GetBlockInfo(uDefines, sDefines);
        
        // Init Block Infos
        InitUniformBlockInfo(uDefines);
        InitSamplerBlockInfo(sDefines);

        // uniform block shader
        backend::UniformBlockInfo bInfos;
        bInfos.at(0) = "targetUniform";
        const std::string UniformShaderBlock = ShaderString::CreateBlockInfo(uDefines, sDefines, bInfos.at(0));

        // vertex shader
        std::string vertexShaderString = ShaderString::GetVertexAttribute();
        vertexShaderString += UniformShaderBlock;
        vertexShaderString += ShaderString::GetBlenderVertexShader(sDefines.size()>0?true:false);

        // fragment shader
        std::string fragmentShaderString = ShaderString::GetFragmentShaderHead();
        fragmentShaderString += UniformShaderBlock;
        if (m_bump)
            ;
        if (m_emission)
            ;
        fragmentShaderString += ShaderString::GetBlenderFragmentShader(UpdateUserMaterialParameters,
            sDefines.size()>0?true:false);

        // Update Program
        m_program = Engine::GetInstance()->GetDriver()->createProgram(
            vertexShaderString, fragmentShaderString, bInfos);

        // bind texture uniform location explicitly
        std::vector<std::string> samplerNames;
        for (size_t i = 0; i < sDefines.size(); i++)
            samplerNames.push_back(sDefines.at(i).name);
        if (samplerNames.size() > 0)
            Engine::GetInstance()->GetDriver()->bindSamplerLocations(m_program,samplerNames);

        // update defaut materialInstance
        UpdateDefaultMaterialInstance();
    }
    uint8_t Material::BaseAlignmentForType(backend::UniformType type) noexcept
    {
        // unit : sizeof(uint32_t)
        switch (type) {
            case backend::UniformType::BOOL:
            case backend::UniformType::FLOAT:
            case backend::UniformType::INT:
            case backend::UniformType::UINT:
                return 1;
            case backend::UniformType::BOOL2:
            case backend::UniformType::FLOAT2:
            case backend::UniformType::INT2:
            case backend::UniformType::UINT2:
                return 2;
            case backend::UniformType::BOOL3:
            case backend::UniformType::BOOL4:
            case backend::UniformType::FLOAT3:
            case backend::UniformType::FLOAT4:
            case backend::UniformType::INT3:
            case backend::UniformType::INT4:
            case backend::UniformType::UINT3:
            case backend::UniformType::UINT4:
            case backend::UniformType::MAT3:
            case backend::UniformType::MAT4:
                return 4;
        }
    }
    uint8_t Material::StrideForType(backend::UniformType type) noexcept
    {
        // unit : sizeof(uint32_t)
        switch (type) {
            case backend::UniformType::BOOL:
            case backend::UniformType::INT:
            case backend::UniformType::UINT:
            case backend::UniformType::FLOAT:
                return 1;
            case backend::UniformType::BOOL2:
            case backend::UniformType::INT2:
            case backend::UniformType::UINT2:
            case backend::UniformType::FLOAT2:
                return 2;
            case backend::UniformType::BOOL3:
            case backend::UniformType::INT3:
            case backend::UniformType::UINT3:
            case backend::UniformType::FLOAT3:
                return 3;
            case backend::UniformType::BOOL4:
            case backend::UniformType::INT4:
            case backend::UniformType::UINT4:
            case backend::UniformType::FLOAT4:
                return 4;
            case backend::UniformType::MAT3:
                return 12;
            case backend::UniformType::MAT4:
                return 16;
        }
    }
}
