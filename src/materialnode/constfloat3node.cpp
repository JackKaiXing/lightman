

#include "materialnode/constfloat3node.h"

namespace lightman
{
    ConstFloat3Node::ConstFloat3Node(const std::string& name, float x, float y, float z)
        : Node(name)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
    }
    void ConstFloat3Node::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        // it is a uniform variable, we add it.
        if (m_enableExpose)
            uDefines.push_back({GetName(), backend::UniformType::FLOAT3, 1, backend::Precision::DEFAULT});
    };
    backend::UniformType ConstFloat3Node::GetShaderString(std::string& result) const
    {
        // it is not a uniform, we declare it and set its initial value here,
        // no need to initial a uniform, since it would be setted outside the shader code.
        if (!m_enableExpose)
            result += "const vec3 " + GetName() + " = vec3(" + std::to_string(m_value[0]) + ", " + 
            std::to_string(m_value[1]) + ", " + std::to_string(m_value[2]) + ");\n"; 

        return backend::UniformType::FLOAT3;
    }
    void ConstFloat3Node::setExposeAsUniform(bool enable)
    {
        m_enableExpose = enable;
    }
} // namespace lightmanConstFloat3Node::
