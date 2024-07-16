
#include <assert.h>

#include "backend/platform.h"

#if defined(WIN32)
    #include "backend/windows/windowsopenglplatform.h"
#elif defined(__APPLE__)
    #include "backend/macos/cocoaopenglplatform.h"
#endif


namespace lightman
{
    namespace backend
    {
        Platform* Platform::Create(BackendType backend)
        {
            if (backend == BackendType::OPENGL)
            {
                // check
                #ifndef LIGHTMAN_OPENGL
                    assert(0);
                #endif
                
                #if defined(WIN32)
                    return new WindowsOpenGLPlatform();
                #elif defined(__APPLE__)
                    return new CocoaOpenGLPlatform();
                #endif
            }
            return nullptr;
        }
    }
}
