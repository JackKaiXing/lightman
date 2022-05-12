
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
    Material::MaterialType Material::StringToMaterialType(const std::string& name)
    {
        // TODO: after verify the path tracing renderer, this will be removed to accompany with real word materials
        if (name.compare("matte") == 0)
            return MaterialType::MATTE;
        else if(name.compare("mirror") == 0)
            return MaterialType::MIRROR;
        else if(name.compare("glass") == 0)
            return MaterialType::GLASS;
        else if(name.compare("glossy2") == 0)
            return MaterialType::GLOSSY2;
        else if(name.compare("archglass") == 0)
            return MaterialType::ARCHGLASS;
        else if(name.compare("mix") == 0)
            return MaterialType::MIX;
        else if(name.compare("nullmat") == 0)
            return MaterialType::NULLMAT;
        else if(name.compare("mattetranslucent") == 0)
            return MaterialType::MATTETRANSLUCENT;
        else if(name.compare("metal2") == 0)
            return MaterialType::METAL2;
        else if(name.compare("roughglass") == 0)
            return MaterialType::ROUGHGLASS;
        else if(name.compare("velvet") == 0)
            return MaterialType::VELVET;
        else if(name.compare("cloth") == 0)
            return MaterialType::CLOTH;
        else if(name.compare("carpaint") == 0)
            return MaterialType::CARPAINT;
        else if(name.compare("roughmatte") == 0)
            return MaterialType::ROUGHMATTE;
        else if(name.compare("roughmattetranslucent") == 0)
            return MaterialType::ROUGHMATTETRANSLUCENT;
        else if(name.compare("glossytranslucent") == 0)
            return MaterialType::GLOSSYTRANSLUCENT;
        else if(name.compare("glossycoating") == 0)
            return MaterialType::GLOSSYCOATING;
        else if(name.compare("disney") == 0)
            return MaterialType::DISNEY;
        else if(name.compare("twosided") == 0)
            return MaterialType::TWOSIDED;
            
        return MaterialType::MAX_MATERIALTYPE_COUNT; // Erro String Name
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
