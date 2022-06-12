
#include "texture/constfloattexture.h"

namespace lightman
{
    ConstFloatTexture::ConstFloatTexture(const std::string& name, float v)
        : Texture(name)
    {
        m_value = v;
    }
    void ConstFloatTexture::GetBlockInfo(std::vector<UniformDefine>& uDefines) const
    {
        if (m_enableExpose)
            uDefines.push_back({GetName(), backend::UniformType::FLOAT, 1, backend::Precision::DEFAULT});
    }
    backend::UniformType ConstFloatTexture::GetShaderString(std::string& result) const
    {
        if (!m_enableExpose)
            result += "const float " + GetName() + " = " + std::to_string(m_value) + ";\n";

        return backend::UniformType::FLOAT;
    }
    void ConstFloatTexture::setExposeAsUniform(bool enable)
    {
        m_enableExpose = enable;
    }
} // namespace lightman
