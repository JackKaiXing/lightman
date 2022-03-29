#ifndef _LIGHTMAN_PLATFORM_H
#define _LIGHTMAN_PLATFORM_H

#include "backend/driverbase.h"

namespace lightman 
{
    namespace backend
    {
        class Driver;
        class Platform
        {
        public:
            static Platform* Create(backend::BackendType backend);
            virtual backend::Driver* CreateDriver() noexcept = 0;
            virtual ~Platform() noexcept{};
        };
    } 
} 
#endif // _LIGHTMAN_PLATFORM_H