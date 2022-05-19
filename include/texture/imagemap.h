#ifndef _LIGHTMAN_IMAGEMAP_H
#define _LIGHTMAN_IMAGEMAP_H

#include <cstdint>
#include <cstddef>

namespace lightman
{
    class Imagemap
    {
    public:
        enum class ComponentType : uint8_t
        {
            R8,
            RGB8,
            RGBA8,
            R32,
            RGB32,
            RGBA32
        };
        enum class ImagemapUsage : uint8_t
        {
            COLOR,
            NORMAL
        };
        static uint32_t GetComponentSize(ComponentType type);
    public:
        Imagemap(void* data, uint32_t w, uint32_t h, ComponentType type, ImagemapUsage usage = ImagemapUsage::COLOR);
        ~Imagemap();
    private:
        void* m_data = nullptr;
        uint32_t m_width, m_height;
    };
} // namespace lightman

#endif  //_LIGHTMAN_IMAGEMAP_H