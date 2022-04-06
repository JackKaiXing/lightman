#ifndef _LIGHTMAN_RENDERER_H
#define _LIGHTMAN_RENDERER_H

#include <cstdint>
#include "engine/engine.h"

using namespace lightman::engine;

namespace lightman
{
    namespace renderer
    {
        //----------------------------------------------------------------------------
        static constexpr uint8_t MAX_RENDERER_TYPE_COUNT = 3;
        enum class RenderType: uint8_t
        {
            RASTER_GPU            = 0,
            PATHTRACING_GPU       = 1,
            PATHTRACING_CPU       = 2,
        };
        //----------------------------------------------------------------------------
        class Renderer
        {
        public:
            Renderer(Engine * engine);
            ~Renderer();
            virtual RenderType GetType() = 0;
            Engine * getEngine() {return m_engine;};
        private:
            Engine * m_engine = nullptr;
        };
        //----------------------------------------------------------------------------
        class GPURenderer : public Renderer
        {
        public:
            GPURenderer(Engine * engine);
            RenderType GetType() override {return RenderType::RASTER_GPU;};
        };
    }
}
#endif // _LIGHTMAN_RENDERER_H