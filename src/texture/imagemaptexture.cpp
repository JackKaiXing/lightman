
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
    backend::UniformType ImagemapTexture::GetShaderString(std::string& result) const
    {
        // TO DO Texture Uniform
            result += "vec3 " + GetName() + " = vec3(0.5);\n"; 

        return backend::UniformType::FLOAT3;
    }
    void ImagemapTexture::setExposeAsUniform(bool enable)
    {
        m_enableExpose = enable;
    }
} // namespace lightman
