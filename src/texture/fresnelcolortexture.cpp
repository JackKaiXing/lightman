
#include "texture/fresnelcolortexture.h"

namespace lightman
{
    void FresnelColorTexture::SetKr(const Texture* kr)
    {
        m_Kr = kr;
    }
} // namespace lightman
