#ifndef _LIGHTMAN_IMAGEMAP_H
#define _LIGHTMAN_IMAGEMAP_H

#include <cstdint>
#include <cstddef>

#include "backend/driverbase.h"
#include "utils/ref.h"

namespace lightman
{
    class Imagemap : public utils::Ref
    {
    public:
        enum class ImagemapUsage : uint8_t
        {
            COLOR,
            NORMAL
        };
        static uint32_t GetComponentSize(backend::TextureFormat type);
    public:
        Imagemap(std::string name, void* data, uint32_t w, uint32_t h, backend::TextureFormat type, 
            backend::PixelDataFormat pixelFormat, backend::PixelDataType pixelType,
            backend::SamplerType samplerType,
            ImagemapUsage usage = ImagemapUsage::COLOR);
        ~Imagemap();

        uint32_t GetWidth(){return m_width;};
        uint32_t GetHeight(){return m_height;};
        backend::TextureFormat GetTextureFormat(){return m_TextureFormatType;};
        backend::SamplerType GetSamplerType(){return m_samplerType;};
        backend::SamplerFormat GetSamplerFormat();
        std::string GetName(){return m_name;};
        void* GetData(){return m_data;};
        uint32_t GetDataSize();
        backend::PixelDataType GetPixelType() {return m_pixelType;};
        backend::PixelDataFormat GetPixelFormat() {return m_pixelFormat;};
        
    private:
        std::string m_name;
        void* m_data = nullptr;
        uint32_t m_width, m_height;
        backend::TextureFormat m_TextureFormatType;
        backend::PixelDataFormat m_pixelFormat;
        backend::PixelDataType m_pixelType;
        backend::SamplerType m_samplerType;
    };
} // namespace lightman

#endif  //_LIGHTMAN_IMAGEMAP_H
