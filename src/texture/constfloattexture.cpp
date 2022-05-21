
#include "texture/constfloattexture.h"

namespace lightman
{
    ConstFloatTexture::ConstFloatTexture(const std::string& name, float v)
        : Texture(name)
    {
        m_value = v;
    }
} // namespace lightman
