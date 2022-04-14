
#include "backend/driverbase.h"

namespace lightman
{
    namespace backend
    {
        size_t Driver::getElementTypeSize(ElementType type) noexcept {
            switch (type) {
                case ElementType::BYTE:     return sizeof(int8_t);
                case ElementType::UBYTE:    return sizeof(uint8_t);
                case ElementType::SHORT:    return sizeof(int16_t);
                case ElementType::USHORT:   return sizeof(uint16_t);
                case ElementType::INT:      return sizeof(int32_t);
                case ElementType::UINT:     return sizeof(uint32_t);
        }
    }
}
#endif // _LIGHTMAN_DRIVERBASE_H