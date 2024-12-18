
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
        struct CocoaGLSwapChain : public Platform::SwapChain
        {
            CocoaGLSwapChain(NSView* inView);
            ~CocoaGLSwapChain() noexcept;

            NSView* view;
        };
        CocoaGLSwapChain::CocoaGLSwapChain( NSView* inView )
        : view(inView)
        {

        }
        CocoaGLSwapChain::~CocoaGLSwapChain() noexcept
        {
            view = nullptr;
        }
        // ----------------------------------------------------------------------------
        // mixing compile C++ and ObjectC
        // https://stackoverflow.com/questions/4714698/mixing-objective-c-m-mm-c-cpp-files
        struct CocoaOpenGLPlatformImpl
        {
            ~CocoaOpenGLPlatformImpl()
            {
                // weak reference, just delete
                m_currentSwapChain = nullptr;
                
                [m_openGLContext dealloc];
            };
            NSOpenGLContext* m_openGLContext = nullptr;
            CocoaGLSwapChain* m_currentSwapChain = nullptr;
        };   
        // ----------------------------------------------------------------------------
        CocoaOpenGLPlatform::CocoaOpenGLPlatform() : m_cocoaImpl(new CocoaOpenGLPlatformImpl())
        {
        }
        CocoaOpenGLPlatform::~CocoaOpenGLPlatform()
        {
            delete m_cocoaImpl;
        }
        void CocoaOpenGLPlatform::DestroyDriver(backend::Driver* targetDriver) noexcept
        {
            if (targetDriver)
            {
                OpenGLDriver* gldriver = static_cast<OpenGLDriver*>(targetDriver);
                delete gldriver;

                bluegl::unbind();
            }
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
            [pixelFormat dealloc];
            
            // https://developer.apple.com/documentation/appkit/nsopenglcpswapinterval
            GLint interval = 0;
            [nsOpenGLContext makeCurrentContext];
            [nsOpenGLContext setValues:&interval forParameter:NSOpenGLCPSwapInterval];

            m_cocoaImpl->m_openGLContext = nsOpenGLContext;

            // https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
            int result = bluegl::bind();
            assert(!result);
            return OpenGLDriver::create(this, nullptr);
        }
        Platform::SwapChain* CocoaOpenGLPlatform::CreateSwainChain(void* nativeWindow)
        {
            printf("CREATE SWAINCHAIN BY COCOAOPENGLDRIVER \n");

            NSView* nsView = (__bridge NSView*)nativeWindow;
            CocoaGLSwapChain* swapChain = new CocoaGLSwapChain( nsView );
            return swapChain;
        }
        void CocoaOpenGLPlatform::DestroySwapChain(Platform::SwapChain* sc)
        {
            if (sc)
            {
                CocoaGLSwapChain* swapChain = static_cast<CocoaGLSwapChain*>(sc);
                delete swapChain;
            }
        }
        void CocoaOpenGLPlatform::MakeCurrent(Platform::SwapChain* drawswapchain)
        {
            CocoaGLSwapChain* swapchain = (CocoaGLSwapChain*)drawswapchain;

            // TODO SWAPCHAIN CHANGE FOR BOUND SIZE
            if(m_cocoaImpl->m_currentSwapChain != swapchain)
            {
                m_cocoaImpl->m_currentSwapChain = swapchain;
                [m_cocoaImpl->m_openGLContext setView:swapchain->view];
                [m_cocoaImpl->m_openGLContext update];
            }
            [m_cocoaImpl->m_openGLContext makeCurrentContext];
        }
        void CocoaOpenGLPlatform::Commit(Platform::SwapChain* drawswapchain)
        {
            // https://developer.apple.com/documentation/appkit/nsopenglcontext/1436211-flushbuffer
            [m_cocoaImpl->m_openGLContext flushBuffer];
        }
    }
}
