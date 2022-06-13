#ifndef _LIGHTMAN_UNIFORMDEFINES_H
#define _LIGHTMAN_UNIFORMDEFINES_H

#include <string>
#include "backend/driverenum.h"
#include "backend/driverbase.h"

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

    struct SamplerDefine
    {
        std::string name;   // sampler name
        std::string imgName;   // img name
        backend::SamplerType type;
        backend::SamplerFormat format;
        bool multiSample = false;
        backend::Precision precision;
    };
}

#endif //_LIGHTMAN_UNIFORMDEFINES_H
