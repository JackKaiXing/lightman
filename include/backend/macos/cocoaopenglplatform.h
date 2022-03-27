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
            void testcocoa();
            backend::Driver* CreateDriver() noexcept override;

        };
    }
}

#endif // _LIGHTMAN_COCOAOPENGLPLATFORM_H