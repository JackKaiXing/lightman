
#include "backend/opengl/opengldriver.h"

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        backend::Driver* OpengGLDriver::create(backend::OpenGLPlatform* platform, void* sharedGLContext) noexcept
        {
            return nullptr;
        }
    }
}