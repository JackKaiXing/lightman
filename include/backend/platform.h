#ifndef _LIGHTMAN_PLATFORM_H
#define _LIGHTMAN_PLATFORM_H

#include "backend/driverenum.h"

namespace lightman 
{
    namespace backend
    {
        class Driver;
        class Platform
        {
        public:
            struct SwapChain {};
            static Platform* Create(backend::BackendType backend);
            virtual backend::Driver* CreateDriver() noexcept = 0;
            virtual Platform::SwapChain* CreateSwainChain(void* nativeWindow) = 0;
            virtual ~Platform() noexcept{};
        };
    } 
} 
#endif // _LIGHTMAN_PLATFORM_H