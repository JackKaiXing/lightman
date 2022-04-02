
#include "platformhandle.h"

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h> // TODO tobe removed
#import <OpenGL/glu.h> // TODO tobe removed

// ----------------------------------------------------------------------------
// VIEW AND CONTEXT
// ----------------------------------------------------------------------------

@interface AppView : NSOpenGLView
{
    NSTimer* animationTimer;
    float accumulator;
}
@end

@implementation AppView

-(float)accumulator
{
    return 0.0f;
}

-(void)prepareOpenGL
{
    [super prepareOpenGL];

    GLint swapInterval = 0;
    [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
}

-(void)updateView
{
    [[self openGLContext] makeCurrentContext];
    accumulator = (accumulator+0.01) > 1.0 ? 0.0 : (accumulator+0.01);
    glClearColor(1.0 * accumulator, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // https://developer.apple.com/documentation/appkit/nsopenglcontext/1436211-flushbuffer
    [[self openGLContext] flushBuffer];

    if (!animationTimer)
        animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}

// https://developer.apple.com/documentation/appkit/nsopenglview/1414948-reshape
-(void)reshape                              { [super reshape]; [[self openGLContext] update]; [self updateView]; }
// https://developer.apple.com/documentation/uikit/uiview/1622529-drawrect
-(void)drawRect:(NSRect)bounds              { [self updateView]; }
-(void)animationTimerFired:(NSTimer*)timer  { [self setNeedsDisplay:YES]; }
-(void)dealloc                              { animationTimer = nil; }

@end

// ----------------------------------------------------------------------------
// WINDOW
// ----------------------------------------------------------------------------
@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, readonly) NSWindow* window;
@end

@implementation AppDelegate
@synthesize window = _window;

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

-(NSWindow*)window
{
    if (_window != nil)
        return (_window);

    NSRect viewRect = NSMakeRect(100.0, 100.0, 100.0 + 500.0, 100 + 500.0);

    _window = [[NSWindow alloc] initWithContentRect:viewRect styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:YES];
    [_window setTitle:@"LightMan MacOs"];
    [_window setAcceptsMouseMovedEvents:YES];
    [_window setOpaque:YES];
    [_window makeKeyAndOrderFront:NSApp];

    return (_window);
}

-(void)dealloc
{
    _window = nil;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Make the application a foreground application (else it won't receive keyboard events)
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        0
    };

    NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    AppView* view = [[AppView alloc] initWithFrame:self.window.frame pixelFormat:format];
    format = nil;

    #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
        if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
            [view setWantsBestResolutionOpenGLSurface:YES];
    #endif // MAC_OS_X_VERSION_MAX_ALLOWED >= 1070

    [self.window setContentView:view];
    if ([view openGLContext] == nil)
        NSLog(@"No OpenGL Context!");
    [view initialize];
}

@end

// ----------------------------------------------------------------------------
// RETRY INTERFACE
// ----------------------------------------------------------------------------
namespace lightman
{
    AppDelegate* delegate = nullptr;
    void* GetNativeWindow()
    {
        return [delegate.window contentView];
    }
    int MainWindow(int argc, const char* argv[], std::function<void()> setup)
    {
        @autoreleasepool
           {
               NSApp = [NSApplication sharedApplication];
               delegate = [[AppDelegate alloc] init];
               setup();
               [[NSApplication sharedApplication] setDelegate:delegate];
               [NSApp run];
           }
           return NSApplicationMain(argc, argv);
    }
}
