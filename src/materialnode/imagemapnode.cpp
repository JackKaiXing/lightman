
#include "texture/imagemaptexture.h"
#include "managers/imagemapmanager.h"

namespace lightman
{
    void ImagemapTexture::SetImageMap(Imagemap* im)
    {
        if(m_imagemap)
            m_imagemap->ReleaseRef();
        
        im->IncreaseRef();
        m_imagemap = im;

        // create default tex
        ImagemapManager::GetInstance()->GetHwTexture(m_imagemap->GetName());
    }
    backend::UniformType ImagemapTexture::GetShaderString(std::string& result) const
    {
        result += "vec3 " + GetName() + " = texture(texture_" + GetName() + ", vUV0).rgb;\n"; 
        return backend::UniformType::FLOAT3;
    }
    void ImagemapTexture::setExposeAsUniform(bool enable)
    {
        m_enableExpose = enable;
    }
    void ImagemapTexture::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        // TODO Do we create this texture based customed parameters?
        sDefines.push_back({"texture_"+ GetName(), 
        m_imagemap->GetName(),
        m_imagemap->GetSamplerType(), 
        m_imagemap->GetSamplerFormat(), 
        false, backend::Precision::DEFAULT});
    };
} // namespace lightman
