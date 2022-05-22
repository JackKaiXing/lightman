#ifndef _LIGHTMAN_RENDERER_H
#define _LIGHTMAN_RENDERER_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include "engine/swapchain.h"
#include "materials/material.h"
#include "objects/instancedtrianglemesh.h"

namespace lightman
{
    class Engine;
    class View;
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
        GPURenderer(uint32_t width, uint32_t height);
        ~GPURenderer();
        RenderType GetType() override {return RenderType::RASTER_GPU;};
        bool BeginFrame(SwapChain *swapChain) override;
        void EndFrame() override;
        void RenderFrame(View* view) override;
    private:
        backend::HwRenderTarget* m_mrt = nullptr; // geometry off-screen rendering
        backend::HwTexture* m_colorTex = nullptr;
        backend::RenderPassParams m_mrtPP;
        backend::HwProgram* m_postprocessing_fxaa = nullptr;
        geometry::TriangleMesh * m_quad = nullptr;
    };
}
#endif // _LIGHTMAN_RENDERER_H
