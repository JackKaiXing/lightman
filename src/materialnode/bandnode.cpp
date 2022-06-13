
#include "materialnode/bandnode.h"

namespace lightman
{
    void BandNode::SetAmount(const Node* amount)
    {
        m_amount = amount;
    }
    void BandNode::AddOffset(float offset)
    {
        m_offsets.push_back(offset);
    }
    void BandNode::AddValue(Color value)
    {
        m_values.push_back(value);
    }
    void BandNode::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        // values and offsets would be seen as const to shaders, would not be changed, so no uniform for bandTexture
        m_amount->GetBlockInfo(uDefines,sDefines); // check amount texture
    }
    bool BandNode::CheckInputType(backend::UniformType type) const
    {
        if (type == backend::UniformType::FLOAT  ||
            type == backend::UniformType::FLOAT3 ||
            type == backend::UniformType::FLOAT4)
        {
            return true;
        }
        return false;
    }
    backend::UniformType BandNode::GetShaderString(std::string& result) const
    {
        backend::UniformType type = m_amount->GetShaderString(result);
        if (CheckInputType(type))
        {
            result += "vec3 " + GetName() + " = vec3(0.0);\n";
            // offset and values are of variable length, we could not create a function to receive this
            // the shader program would be big if there are multiple bandTextures
            // use bracket to avoid repeated variable name
            result += "{ "; 
            result += "const int size = " + std::to_string(m_offsets.size()) + ";\n";
            result += "float offsets[size] = float[size](";
            for (size_t i = 0; i < m_offsets.size(); i++)
            {
                result += std::to_string(m_offsets.at(i));
                if (i != m_offsets.size() - 1)
                     result += ", ";
            }
            result += ");\n";

            result += "vec3 values[size] = vec3[size](";
            for (size_t i = 0; i < m_values.size(); i++)
            {
                Color value = m_values.at(i);
                result += "vec3(" + 
                    std::to_string(value.GetR()) + ", " + 
                    std::to_string(value.GetG()) + ", " + 
                    std::to_string(value.GetB()) + ")";
                if (i != m_offsets.size() - 1)
                     result += ", ";
            }
            result += ");\n";
            result += "float a = 0.0;\n";
            if (type == backend::UniformType::FLOAT)
            {
                result += "a = " + m_amount->GetName() + ";\n"; 
            }
            else if (type == backend::UniformType::FLOAT3)
            {
                result += "a = Luminance(" + m_amount->GetName() + ");\n"; 
            }
            else if (type == backend::UniformType::FLOAT4)
            {
                result += "a = Luminance(" + m_amount->GetName() + ".rgb);\n"; 
            }
            result += "a = clamp(a, 0.0, 1.0);\n if (a < offsets[0])";
            result += GetName() + "= values[0];\n";
		    result += "if (a >= offsets[size-1])";
	        result += GetName() + "= values[size-1];\n";
            result += "int p = 0; \n \
	            for (; p < size; ++p) {  \n \
                    if (a < offsets[p])  \n \
                        break;  \n \
                } \n \
                float factor = (a - offsets[p - 1]) / (offsets[p] - offsets[p - 1]);\n";
            result += GetName() + "= values[p-1];\n";
            result += "}\n";
        }

        return backend::UniformType::FLOAT3;
        
    }
} // namespace lightman
