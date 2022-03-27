#ifndef _LIGHTMAN_RENDERER_H
#define _LIGHTMAN_RENDERER_H

#include <cstdint>

namespace lightman
{
    namespace renderer
    {
        static constexpr uint8_t MAX_RENDERER_TYPE_COUNT = 3;
        enum class RenderType: uint8_t
        {
            RASTER_GPU            = 0,
            PATHTRACING_GPU       = 1,
            PATHTRACING_CPU       = 2,
        };

        class Renderer
        {
        public:
            
        };
    }
}
#endif // _LIGHTMAN_RENDERER_H