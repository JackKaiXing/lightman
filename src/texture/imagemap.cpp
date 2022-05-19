
#include "texture/imagemap.h"
#include  <cstring>
#include  <stdlib.h>

namespace lightman
{
    uint32_t Imagemap::GetComponentSize(ComponentType type)
    {
        switch (type)
        {
        case ComponentType::R8:
            return 1;
            break;
        case ComponentType::RGB8:
            return 3;
            break;
        case ComponentType::RGBA8:
            return 1;
            break;
        case ComponentType::R32:
            return 4;
            break;
        case ComponentType::RGB32:
            return 12;
            break;
        case ComponentType::RGBA32:
            return 16;
            break;
        
        default:
            break;
        }
    }
    Imagemap::Imagemap(void* data, uint32_t w, uint32_t h, ComponentType type, ImagemapUsage usage)
    {
        uint32_t size = w * h * GetComponentSize(type);
        m_data = (void*)malloc(size);
        std::memcpy(m_data, data, size);

        m_width = w;
        m_height = h;
    }
    Imagemap::~Imagemap()
    {
        if (m_data)
            free(m_data);
    }
}