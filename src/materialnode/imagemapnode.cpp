
#include "materialnode/imagemapnode.h"
#include "managers/imagemapmanager.h"

namespace lightman
{
    ImagemapNode::~ImagemapNode()
    {
        if (m_imagemap)
            RELEASEORDELETE(m_imagemap);
    }
    void ImagemapNode::SetImageMap(Imagemap* im)
    {
        if(m_imagemap)
            RELEASEORDELETE(m_imagemap);
        
        m_imagemap = im;
        m_imagemap->IncreaseRef();
    }
    backend::UniformType ImagemapNode::GetShaderString(std::string& result) const
    {
        result += "vec3 " + GetName() + " = texture(texture_" + GetName() + ", vUV0).rgb;\n"; 
        return backend::UniformType::FLOAT3;
    }
    void ImagemapNode::setExposeAsUniform(bool enable)
    {
        m_enableExpose = enable;
    }
    void ImagemapNode::GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const
    {
        sDefines.push_back({"texture_"+ GetName(), 
            m_imagemap->GetName(),
            m_imagemap->GetSamplerType(), 
            m_imagemap->GetSamplerFormat(), 
            false, backend::Precision::DEFAULT});
    };
} // namespace lightman
