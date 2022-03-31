
#include "backend/macos/cocoaopenglplatform.h"

#include <stdio.h>
#include <bluegl/BlueGL.h>

#include <OpenGL/OpenGL.h>
#include <Cocoa/Cocoa.h>

#include "backend/opengl/opengldriver.h"

namespace lightman
{
    namespace backend
    {
        // ----------------------------------------------------------------------------
        // mixing compile C++ and ObjectC
        // https://stackoverflow.com/questions/4714698/mixing-objective-c-m-mm-c-cpp-files
        struct CocoaOpenGLPlatformImpl
        {
            // TODO: METAL, OpenGL is deprecatingn
            NSOpenGLContext* m_openGLContext = nullptr;
        };   
        // ----------------------------------------------------------------------------
        CocoaOpenGLPlatform::CocoaOpenGLPlatform() : m_cocoaImpl(new CocoaOpenGLPlatformImpl())
        {
        }
        CocoaOpenGLPlatform::~CocoaOpenGLPlatform()
        {
            delete m_cocoaImpl;
        }
        backend::Driver* CocoaOpenGLPlatform::CreateDriver() noexcept
        {
            // https://developer.apple.com/documentation/appkit/nsopenglpixelformat/1436219-initwithattributes?language=objc
            // https://developer.apple.com/documentation/appkit/1436213-opengl_pixel_format_attributes/nsopenglpfaaccelerated
            // https://developer.apple.com/documentation/appkit/nsopenglpfanorecovery
            // NSOpenGLPFAColorSize: when unspecified, a format that matches the screen is preferred
            // unlike windows, no need for device context for creating render context
            NSOpenGLPixelFormatAttribute pixelFormatAttributes[] = {
                    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
                    NSOpenGLPFADepthSize,    (NSOpenGLPixelFormatAttribute) 24,
                    NSOpenGLPFADoubleBuffer, (NSOpenGLPixelFormatAttribute) true,
                    NSOpenGLPFAAccelerated,  (NSOpenGLPixelFormatAttribute) true,
                    NSOpenGLPFANoRecovery,   (NSOpenGLPixelFormatAttribute) true,
                    0, 0,
            };
            
            NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes];
            NSOpenGLContext* nsOpenGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nullptr];
            
            // https://developer.apple.com/documentation/appkit/nsopenglcpswapinterval
            GLint interval = 0;
            [nsOpenGLContext makeCurrentContext];
            [nsOpenGLContext setValues:&interval forParameter:NSOpenGLCPSwapInterval];

            m_cocoaImpl->m_openGLContext = nsOpenGLContext;

            // https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
            int result = bluegl::bind();
            assert(!result);
            OpengGLDriver::create(this, nullptr);
        }
    }
}
