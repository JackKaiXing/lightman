#ifndef _LIGHTMAN_COCOAOPENGLPLATFORM_H
#define _LIGHTMAN_COCOAOPENGLPLATFORM_H

#include "backend/opengl/openglplatform.h"

namespace lightman
{
    namespace backend
    {
        struct CocoaOpenGLPlatformImpl;
        // ----------------------------------------------------------------------------
        class CocoaOpenglPlatform final : public OpenglPlatform
        {
        public:
            backend::Driver* CreateDriver() noexcept override;
            CocoaOpenglPlatform();
            ~CocoaOpenglPlatform();
        private:
            CocoaOpenGLPlatformImpl * m_cocoaImpl;
        };
    }
}

#endif // _LIGHTMAN_COCOAOPENGLPLATFORM_H
