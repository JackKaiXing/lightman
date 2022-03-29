#ifndef _LIGHTMAN_COCOAOPENGLPLATFORM_H
#define _LIGHTMAN_COCOAOPENGLPLATFORM_H

#include "backend/opengl/openglplatform.h"

namespace lightman
{
    namespace backend
    {
        class CocoaOpenglPlatform final : public OpenglPlatform
        {
        public:
            backend::Driver* CreateDriver() noexcept override;
            CocoaOpenglPlatform();
            ~CocoaOpenglPlatform(){};
        };
    }
}

#endif // _LIGHTMAN_COCOAOPENGLPLATFORM_H