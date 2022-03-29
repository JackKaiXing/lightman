
#include <stdio.h>

#include "backend/macos/cocoaopenglplatform.h"

#include <OpenGL/OpenGL.h>
#include <Cocoa/Cocoa.h>

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        // 
        // https://stackoverflow.com/questions/4714698/mixing-objective-c-m-mm-c-cpp-files
        struct CocoaOpenGLPlatformImpl
        {
            NSOpenGLContext* m_openGLContext = nullptr;
        };   
        // ----------------------------------------------------------------------------
        CocoaOpenglPlatform::CocoaOpenglPlatform() : m_cocoaImpl(new CocoaOpenGLPlatformImpl())
        {
        }
        CocoaOpenglPlatform::~CocoaOpenglPlatform()
        {
            delete m_cocoaImpl;
        }
        backend::Driver* CocoaOpenglPlatform::CreateDriver() noexcept
        {
            return nullptr;
        }
    }
}
