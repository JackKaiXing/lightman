#ifndef _LIGHTMAN_COCOAOPENGLPLATFORM_H
#define _LIGHTMAN_COCOAOPENGLPLATFORM_H

#include "backend/opengl/openglplatform.h"

namespace lightman
{
    namespace backend
    {
        struct CocoaOpenGLPlatformImpl;
        // ----------------------------------------------------------------------------
        class CocoaOpenGLPlatform final : public OpenGLPlatform
        {
        public:
            backend::Driver* CreateDriver() noexcept override;
            CocoaOpenGLPlatform();
            ~CocoaOpenGLPlatform();
        private:
            CocoaOpenGLPlatformImpl * m_cocoaImpl;
        };
    }
}

#endif // _LIGHTMAN_COCOAOPENGLPLATFORM_H
