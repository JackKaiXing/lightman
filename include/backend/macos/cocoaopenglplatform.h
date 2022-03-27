#ifndef _LIGHTMAN_COCOAOPENGLPLATFORM_H
#define _LIGHTMAN_COCOAOPENGLPLATFORM_H

#include "backend/opengl/openglplatform.h"

namespace lightman
{
    namespace backend
    {
        class CocoaOpenglPlatform : public OpenglPlatform
        {
        public:
            void testcocoa();
        };
    }
}

#endif // _LIGHTMAN_COCOAOPENGLPLATFORM_H