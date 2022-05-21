

#include "texture/constfloat3texture.h"

namespace lightman
{
    ConstFloat3Texture::ConstFloat3Texture(const std::string& name, float x, float y, float z)
        : Texture(name)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
    }
} // namespace lightmanConstFloat3Texture::