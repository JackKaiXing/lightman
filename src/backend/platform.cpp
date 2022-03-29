
#include "backend/platform.h"
#include "backend/macos/cocoaopenglplatform.h"

namespace lightman
{
    namespace backend
    {
        Platform* Platform::Create(BackendType backend)
        {
            #if defined(WIN32)
                return nullptr;
            #elif defined(__APPLE__)
                return new CocoaOpenglPlatform();
            #endif
            
            return nullptr;
        }
    }
}
