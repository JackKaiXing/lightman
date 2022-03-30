
#include "backend/platform.h"
#include "backend/macos/cocoaopenglplatform.h"

namespace lightman
{
    namespace backend
    {
        Platform* Platform::Create(BackendType backend)
        {
            if (backend == BackendType::OPENGL)
            {
                #if defined(WIN32)
                    return nullptr;
                #elif defined(__APPLE__)
                    return new CocoaOpenGLPlatform();
                #endif
            }
            return nullptr;
        }
    }
}
