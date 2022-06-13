
#include "materialnode/constfloatnode.h"

namespace lightman
{
    ConstFloatNode::ConstFloatNode(const std::string& name, float v)
        : Node(name)
    {
        m_value = v;
    }
    void ConstFloatNode::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        if (m_enableExpose)
            uDefines.push_back({GetName(), backend::UniformType::FLOAT, 1, backend::Precision::DEFAULT});
    }
    backend::UniformType ConstFloatNode::GetShaderString(std::string& result) const
    {
        if (!m_enableExpose)
            result += "const float " + GetName() + " = " + std::to_string(m_value) + ";\n";

        return backend::UniformType::FLOAT;
    }
    void ConstFloatNode::setExposeAsUniform(bool enable)
    {
        m_enableExpose = enable;
    }
} // namespace lightman
