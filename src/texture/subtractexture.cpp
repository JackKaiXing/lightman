
#include "texture/subtracttexture.h"

namespace lightman
{
    void SubtractTexture::SetTex1(const Texture * tex1)
    {
        m_tex1 = tex1;
    }
    void SubtractTexture::SetTex2(const Texture * tex2)
    {
        m_tex2 = tex2;
    }
    void SubtractTexture::GetBlockInfo(std::vector<UniformDefine>& uDefines) const
    {
        m_tex1->GetBlockInfo(uDefines);
        m_tex2->GetBlockInfo(uDefines);
    }
    backend::UniformType SubtractTexture::GetShaderString(std::string& result) const
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