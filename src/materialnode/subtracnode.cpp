
#include "materialnode/subtractnode.h"

namespace lightman
{
    void SubtractNode::SetTex1(const Node * tex1)
    {
        m_tex1 = tex1;
    }
    void SubtractNode::SetTex2(const Node * tex2)
    {
        m_tex2 = tex2;
    }
    void SubtractNode::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        m_tex1->GetBlockInfo(uDefines, sDefines);
        m_tex2->GetBlockInfo(uDefines, sDefines);
    }
    backend::UniformType SubtractNode::GetShaderString(std::string& result) const
    {
        // declare first
        backend::UniformType type1 = m_tex1->GetShaderString(result);
        backend::UniformType type2 = m_tex2->GetShaderString(result);

        // use declaration
        backend::UniformType target;
        if (CheckTypeForBinaryOperation(type1, type2, target))
        {
            result += ShaderString::UniformTypeToShaderString(target) + " " + GetName() + 
                " = " + m_tex1->GetName() + " - " + m_tex2->GetName() + ";\n";
        }
        else
            assert(0);

        return target;
    }
} // namespace lightman
