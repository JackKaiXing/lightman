#ifndef _LIGHTMAN_MATERIALS_H
#define _LIGHTMAN_MATERIALS_H

#include "backend/driverbase.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class Material
    {
    public:
        // for convenient usage of enum, https://riptutorial.com/c/example/6564/typedef-enum
        typedef enum
        {
            MATTE, GLASS
        }MaterialType;
    public:
        Material() = default;
        virtual ~Material() = default;
        virtual MaterialType getMaterialType() = 0;
    protected:
        backend::HwProgram * m_program = nullptr;
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALS_H