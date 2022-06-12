
#include "texture/mixtexture.h"

namespace lightman
{
    void MixTexture::SetAmount(const Texture * amount)
    {
        m_amount = amount;
    }
    void MixTexture::SetTex1(const Texture * tex1)
    {
        m_tex1 = tex1;
    }
    void MixTexture::SetTex2(const Texture * tex2)
    {
        m_tex2 = tex2;
    }
    void MixTexture::GetBlockInfo(std::vector<UniformDefine>& uDefines) const
    {
        m_amount->GetBlockInfo(uDefines);
	    m_tex1->GetBlockInfo(uDefines);
	    m_tex2->GetBlockInfo(uDefines);
    }
    backend::UniformType MixTexture::GetShaderString(std::string& result) const
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
