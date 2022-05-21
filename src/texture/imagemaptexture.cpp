
#include "texture/imagemaptexture.h"

namespace lightman
{
    void ImagemapTexture::SetImageMap(Imagemap* im)
    {
        if(m_imagemap)
            m_imagemap->ReleaseRef();
        
        im->IncreaseRef();
        m_imagemap = im;
    }
} // namespace lightman
