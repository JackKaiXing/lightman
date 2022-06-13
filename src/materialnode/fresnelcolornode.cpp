
#include "texture/fresnelcolortexture.h"

namespace lightman
{
    void FresnelColorTexture::SetKr(const Texture* kr)
    {
        m_Kr = kr;
    }
    void FresnelColorTexture::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const 
    {
        m_Kr->GetBlockInfo(uDefines, sDefines);
    }
    backend::UniformType FresnelColorTexture::GetShaderString(std::string& result) const
    {
        backend::UniformType target = m_Kr->GetShaderString(result);

        result += ShaderString::UniformTypeToShaderString(target) + " " + GetName() + 
                " = " + m_Kr->GetName() + ";\n";
    }
} // namespace lightman
