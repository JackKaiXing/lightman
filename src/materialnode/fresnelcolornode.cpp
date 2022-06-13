
#include "materialnode/fresnelcolornode.h"

namespace lightman
{
    void FresnelColorNode::SetKr(const Node* kr)
    {
        m_Kr = kr;
    }
    void FresnelColorNode::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const 
    {
        m_Kr->GetBlockInfo(uDefines, sDefines);
    }
    backend::UniformType FresnelColorNode::GetShaderString(std::string& result) const
    {
        backend::UniformType target = m_Kr->GetShaderString(result);

        result += ShaderString::UniformTypeToShaderString(target) + " " + GetName() + 
                " = " + m_Kr->GetName() + ";\n";
    }
} // namespace lightman
