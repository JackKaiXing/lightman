#ifndef _LIGHTMAN_UNIFORMDEFINES_H
#define _LIGHTMAN_UNIFORMDEFINES_H

#include <string>
#include "backend/driverenum.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    struct UniformDefine
    {
        std::string name;
        backend::UniformType type;
        uint32_t size;
        backend::Precision precision;
    };
}

#endif //_LIGHTMAN_UNIFORMDEFINES_H
