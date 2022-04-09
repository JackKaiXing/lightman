#ifndef _LIGHTMAN_VIEWPORT_H
#define _LIGHTMAN_VIEWPORT_H

#include <cstdint>

namespace lightman
{
    class Viewport
    {
    public:
        Viewport();
        Viewport(int32_t left, int32_t bottom, uint32_t m_width,uint32_t m_height);
        virtual ~Viewport();
    private:
        int32_t m_left;
        int32_t m_bottom;
        uint32_t m_width;
        uint32_t m_height;
    };
}
#endif // VIEWPORT