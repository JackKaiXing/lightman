
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
} // namespace lightman