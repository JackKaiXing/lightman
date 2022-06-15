
#include "materialnode/mixnode.h"

namespace lightman
{
    MixNode::~MixNode()
    {
        if(m_amount)
            delete m_amount;
        if(m_tex1)
            delete m_tex1;
        if(m_tex2)
            delete m_tex2;
    }
    void MixNode::SetAmount(const Node * amount)
    {
        m_amount = amount;
    }
    void MixNode::SetTex1(const Node * tex1)
    {
        m_tex1 = tex1;
    }
    void MixNode::SetTex2(const Node * tex2)
    {
        m_tex2 = tex2;
    }
    void MixNode::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        m_amount->GetBlockInfo(uDefines, sDefines);
	    m_tex1->GetBlockInfo(uDefines, sDefines);
	    m_tex2->GetBlockInfo(uDefines, sDefines);
    }
    backend::UniformType MixNode::GetShaderString(std::string& result) const
    {
        backend::UniformType type0 = m_amount->GetShaderString(result);
        backend::UniformType type1 = m_tex1->GetShaderString(result);
        backend::UniformType type2 = m_tex2->GetShaderString(result);

        backend::UniformType temp;
        backend::UniformType target;
        if (CheckTypeForBinaryOperation(type1, type2, temp))
        {
            if(CheckTypeForBinaryOperation(type0, temp, target))
            {
                result += ShaderString::UniformTypeToShaderString(target) + " " + GetName() + 
                " = mix(" + m_tex1->GetName() + ", " + m_tex2->GetName() + ", " + m_amount->GetName() + ");\n";
            }
            else
                assert(0);
        }
        else
            assert(0);

        return target;
    }
} // namespace lightman
