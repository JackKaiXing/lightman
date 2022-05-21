
#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    Material::Material(const std::string& name)
    {
        m_name = name;
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
    bool Material::HasUniform(const std::string name)
    {
        return m_uniformInfoMap.find(name) != m_uniformInfoMap.end();
    }
    void Material::GetUniformOffsetAndStrideByName(const std::string name, uint32_t& offset, uint32_t& stride)
    {
        uint32_t index = m_uniformInfoMap.find(name)->second;
        if (index >=0 && index < m_uniformsInfoList.size())
        {
            stride = sizeof(uint32_t) * m_uniformsInfoList[index].stride;
            offset = sizeof(uint32_t) * m_uniformsInfoList[index].offset;
        }
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
