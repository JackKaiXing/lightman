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
            virtual void DestroySwapChain(Platform::SwapChain* sc) = 0;
            virtual ~Platform() noexcept{};
            virtual void MakeCurrent(Platform::SwapChain* swapchain) = 0;
            virtual void Commit(Platform::SwapChain* swapchain) = 0;
        };
    } 
} 
#endif // _LIGHTMAN_PLATFORM_H