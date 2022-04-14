
#include "backend/driverbase.h"

namespace lightman
{
    namespace backend
    {
        size_t Driver::getElementTypeSize(ElementType type) noexcept {
            switch (type) {
                case ElementType::BYTE:     return sizeof(int8_t);
                case ElementType::BYTE2:    return sizeof(int8_t)*2;
                case ElementType::BYTE3:    return sizeof(int8_t)*3;
                case ElementType::BYTE4:    return sizeof(int8_t)*4;
                case ElementType::UBYTE:    return sizeof(uint8_t);
                case ElementType::UBYTE2:   return sizeof(uint8_t)*2;
                case ElementType::UBYTE3:   return sizeof(uint8_t)*3;
                case ElementType::UBYTE4:   return sizeof(uint8_t)*4;
                case ElementType::SHORT:    return sizeof(int16_t);
                case ElementType::SHORT2:   return sizeof(int16_t)*2;
                case ElementType::SHORT3:   return sizeof(int16_t)*3;
                case ElementType::SHORT4:   return sizeof(int16_t)*4;
                case ElementType::USHORT:   return sizeof(uint16_t);
                case ElementType::USHORT2:  return sizeof(uint16_t)*2;
                case ElementType::USHORT3:  return sizeof(uint16_t)*3;
                case ElementType::USHORT4:  return sizeof(uint16_t)*4;
                case ElementType::INT:      return sizeof(int32_t);
                case ElementType::UINT:     return sizeof(uint32_t);
                case ElementType::FLOAT:    return sizeof(float);
                case ElementType::FLOAT2:   return sizeof(float)*2;
                case ElementType::FLOAT3:   return sizeof(float)*3;
                case ElementType::FLOAT4:   return sizeof(float)*4;
            }
        }
    }
}