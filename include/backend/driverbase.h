#ifndef _LIGHTMAN_DRIVERBASE_H
#define _LIGHTMAN_DRIVERBASE_H

#include <cstddef>
#include <cstdint>
#include <assert.h>
#include <array>
#include <string>

#include "backend/platform.h"
#include "math/vector.h"
#include "view/viewport.h"

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        // 
        // ----------------------------------------------------------------------------
        struct HwBase
        {
            
        };
        
        // ----------------------------------------------------------------------------
        // Vertex Buffer Definition
        // ----------------------------------------------------------------------------
        struct Attribute
        {
            static constexpr uint8_t FLAG_NORMALIZED     = 0x1;
            static constexpr uint8_t FLAG_INTEGER_TARGET = 0x2;
            static constexpr uint8_t BUFFER_UNUSED = 0xFF;

            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
            ElementType type = ElementType::BYTE;   //
            uint8_t flags = 0x0;                    // to use as integer value, ((flags & FLAG_INTEGER_TARGET)>0)
                                                    // to use as float value, ((flags & FLAG_INTEGER_TARGET)<0)
                                                    // to use as normalized float value, ((flags & FLAG_INTEGER_TARGET)<0, (flags & FLAG_NORMALIZED)>0)
                                                    // This is because of different Driver-API calls.
            uint8_t stride = 0;                     // in bytes
            uint32_t offset = 0;                    // in bytes
            uint8_t buffer = BUFFER_UNUSED;         // buffer index
        };
        using AttributeArray = std::array<Attribute, MAX_VERTEX_ATTRIBUTE_COUNT>;
        struct HwVertexBuffer : public HwBase
        {
            AttributeArray attributes{};
            uint32_t vertexCount{};
        };

        // ----------------------------------------------------------------------------
        // Index Buffer Definition
        // ----------------------------------------------------------------------------
        struct HwIndexBuffer : public HwBase
        {
            // https://en.cppreference.com/w/cpp/language/bit_field
            uint32_t count : 27;
            uint32_t elementSize : 5; // USHORT, UINT..

            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
            // size = count * elementSize in bytes
            HwIndexBuffer() noexcept : count{}, elementSize{} { };
            HwIndexBuffer(uint8_t elementSize, uint32_t indexCount) noexcept :
                    count(indexCount), elementSize(elementSize) {
                assert(elementSize > 0 && elementSize <= 16);
                assert(indexCount < (1u << 27));
            }
        };
        // ----------------------------------------------------------------------------
        // Buffer Objects
        // ----------------------------------------------------------------------------
        struct HwBufferObject : public HwBase {
            uint32_t byteCount{};
        };
        // ----------------------------------------------------------------------------
        // Render Primitives
        // ----------------------------------------------------------------------------
        struct HwRenderPrimitive : public HwBase
        {
            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawRangeElements.xhtml
            PrimitiveType type = PrimitiveType::TRIANGLES;          // draw mode
            uint32_t minIndex{};                                    // start index
            uint32_t maxIndex{};                                    // end index
            uint32_t count{};                                       // number of elements
            uint32_t offset{};                                      // a pointer to the location where the indices are stored.
        };
        // ----------------------------------------------------------------------------
        // Program
        // ----------------------------------------------------------------------------
        struct HwProgram : public HwBase
        {
            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
            std::string name;
            explicit HwProgram(std::string name) noexcept : name(std::move(name)) { }
            HwProgram() noexcept = default;
        };
        // ----------------------------------------------------------------------------
        // SwapChain
        // ----------------------------------------------------------------------------
        struct HwSwapChain : public HwBase
        {
            Platform::SwapChain * swapchain = nullptr;
        };
        // ----------------------------------------------------------------------------
        // RenderTarget
        // ----------------------------------------------------------------------------
        struct HwRenderTarget : public HwBase
        {
            uint32_t width{};
            uint32_t height{};
            static constexpr uint8_t MAX_SUPPORTED_RENDER_TARGET_COUNT = 8u;
            HwRenderTarget() noexcept = default;
            HwRenderTarget(uint32_t w, uint32_t h) : width(w), height(h) { };
        };
        // ----------------------------------------------------------------------------
        // Texture
        // ----------------------------------------------------------------------------
        struct HwTexture : public HwBase
        {
            uint32_t width{};
            uint32_t height{};
            uint32_t depth{};
            SamplerType target{};
            uint8_t levels : 4;  // This allows up to 15 levels (max texture size of 32768 x 32768)
            uint8_t samples : 4; // Sample count per pixel (should always be a power of 2)
            TextureFormat format{};
            TextureUsage usage{};
        };
        // ----------------------------------------------------------------------------
        // DriverBase
        // ----------------------------------------------------------------------------
        class TargetBufferInfo
        {
        public:
            // 2D textures
            TargetBufferInfo(HwTexture* tex, uint8_t level = 0) noexcept
                : m_tex(tex), m_level(level) { };
            ~TargetBufferInfo() {m_tex = nullptr; m_level = 0;};
            TargetBufferInfo() noexcept { };
            
            // texture to be used as render target
            HwTexture* m_tex = nullptr;
            // level to be used
            uint8_t m_level = 0;
        };

        class MRT
        {
        private:
            TargetBufferInfo mInfos[HwRenderTarget::MAX_SUPPORTED_RENDER_TARGET_COUNT];

        public:
            TargetBufferInfo const& operator[](size_t i) const noexcept {
                return mInfos[i];
            }

            TargetBufferInfo& operator[](size_t i) noexcept {
                return mInfos[i];
            }

            MRT() noexcept = default;

            MRT(TargetBufferInfo const& color) noexcept
                    : mInfos{ color } {
            }
        };

        struct RenderPassFlags {
            // bitmask indicating which buffers to clear at the beginning of a render pass.
            TargetBufferFlags clear;
        };

        struct RenderPassParams
        {
            RenderPassFlags flags{};            // clear options
            lightman::Viewport viewport{};                //
            math::Vector4 clearColor = {};       // clear color  value
            double clearDepth = 0.0;            // clear depth  value
            uint32_t clearStencil = 0;          // clear stencil value
        };

        class Driver
        {
        public:
            #define DECL_DRIVER_API(methodName, paramsDecl, params) virtual void methodName(paramsDecl) = 0;
            #define DECL_DRIVER_API_RETURN(RetType, methodName, paramsDecl, params) virtual RetType methodName(paramsDecl) = 0;
            #include "backend/driverapi.inc"
        public:
            static size_t getElementTypeSize(ElementType type) noexcept;
        };
    }
}
#endif // _LIGHTMAN_DRIVERBASE_H
