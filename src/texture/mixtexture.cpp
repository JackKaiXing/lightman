
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
} // namespace lightman
