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
            Platform(Platform const&) = delete;
            Platform(Platform&&) = delete;
            Platform& operator=(Platform const&) = delete;
            Platform& operator=(Platform&&) = delete;
        
        protected:
            Platform() noexcept;
            virtual ~Platform();
        };
    } 
} 
#endif // _LIGHTMAN_PLATFORM_H