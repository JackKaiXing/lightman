
#include "texture/scaletexture.h"

namespace lightman
{
    void ScaleTexture::SetTex1(const Texture * tex1)
    {
        m_tex1 = tex1;
    }
    void ScaleTexture::SetTex2(const Texture * tex2)
    {
        m_tex2 = tex2;
    }
    void ScaleTexture::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        m_tex1->GetBlockInfo(uDefines, sDefines);
        m_tex2->GetBlockInfo(uDefines, sDefines);
    }
    backend::UniformType ScaleTexture::GetShaderString(std::string& result) const
    {
        backend::UniformType type1 = m_tex1->GetShaderString(result);
        backend::UniformType type2 = m_tex2->GetShaderString(result);

        backend::UniformType target;
        if (CheckTypeForBinaryOperation(type1, type2, target))
        {
            result += ShaderString::UniformTypeToShaderString(target) + " " + GetName() + 
                " = " + m_tex1->GetName() + " * " + m_tex2->GetName() + ";\n";
        }
        else
            assert(0);

        return target;
    }

} // namespace lightman
