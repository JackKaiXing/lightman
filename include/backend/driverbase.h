#ifndef _LIGHTMAN_DRIVERBASE_H
#define _LIGHTMAN_DRIVERBASE_H

#include <cstddef>
#include <cstdint>
#include <assert.h>
#include <array>
#include <string>

#include "backend/platform.h"

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
            uint32_t elementSize : 5;

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
        // DriverBase
        // ----------------------------------------------------------------------------
        class Driver
        {
        public:
            #define DECL_DRIVER_API(methodName, paramsDecl, params) virtual void methodName(paramsDecl) = 0;
            #define DECL_DRIVER_API_RETURN(RetType, methodName, paramsDecl, params) virtual RetType methodName(paramsDecl) = 0;
            #include "backend/driverapi.inc"
        };
    }
}
#endif // _LIGHTMAN_DRIVERBASE_H