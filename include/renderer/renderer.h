#ifndef _LIGHTMAN_RENDERER_H
#define _LIGHTMAN_RENDERER_H

#include <cstdint>
#include "engine/engine.h"
#include "view/view.h"

namespace lightman
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
        virtual RenderType GetType() = 0;
        virtual bool BeginFrame(SwapChain *SwapChain) = 0;
        virtual void EndFrame() = 0;
        virtual void RenderFrame(View* view) = 0;
    protected:
        Renderer() = default;
        virtual ~Renderer();
        SwapChain * m_swapChain = nullptr;
    };
    //----------------------------------------------------------------------------
    class GPURenderer : public Renderer
    {
    public:
        GPURenderer();
        ~GPURenderer();
        RenderType GetType() override {return RenderType::RASTER_GPU;};
        bool BeginFrame(SwapChain *swapChain) override;
        void EndFrame() override;
        void RenderFrame(View* view) override;
    };
}
#endif // _LIGHTMAN_RENDERER_H