
#include "materialnode/imagemap.h"
#include  <cstring>
#include  <stdlib.h>

#include "engine/engine.h"

namespace lightman
{
    uint32_t Imagemap::GetComponentSize(backend::TextureFormat type)
    {
        switch (type)
        {
        case backend::TextureFormat::R8:
            return 1;
            break;
        case backend::TextureFormat::RGB8:
            return 3;
            break;
        case backend::TextureFormat::RGBA8:
            return 1;
            break;
        
        default:
            break;
        }
    }
    backend::SamplerFormat Imagemap::GetSamplerFormat()
    {
        return backend::SamplerFormat::FLOAT;
    }
    Imagemap::Imagemap(std::string name, void* data, uint32_t w, uint32_t h, backend::TextureFormat type, 
        backend::PixelDataFormat pixelFormat, backend::PixelDataType pixelType,
        backend::SamplerType samplerType, ImagemapUsage usage)
    {
        uint32_t size = w * h * GetComponentSize(type);
        m_data = (void*)malloc(size);
        std::memcpy(m_data, data, size);

        m_name = name;
        m_samplerType = samplerType;
        m_TextureFormatType = type;
        m_width = w;
        m_height = h;
        m_pixelFormat = pixelFormat;
        m_pixelType = pixelType;
    }
    backend::HwTexture* Imagemap::GetHwTexture()
    {
        if (!m_hwTexture)
        {
            m_hwTexture = Engine::GetInstance()->GetDriver()->createTexture(backend::SamplerType::SAMPLER_2D, 
                    1, 
                    m_TextureFormatType, 1, 
                    m_width, 
                    m_height, 
                    1, 
                    backend::TextureUsage::SAMPLEABLE);
            Engine::GetInstance()->GetDriver()->update2DImage(m_hwTexture, 0,
                0, 0,
                m_width, m_height,
                m_data, GetDataSize(), 
                m_pixelFormat, m_pixelType);
        }
        
        return m_hwTexture;
    }
    uint32_t Imagemap::GetDataSize()
    {
        return m_width * m_height * GetComponentSize(m_TextureFormatType);
    }
    Imagemap::~Imagemap()
    {
        RELEASEANDRETURN(this);

        if (m_data)
            free(m_data);
        if (m_hwTexture)
            delete m_hwTexture;
    }
}
