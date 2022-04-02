
#include "platformhandle.h"

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h> // to be removed
#import <OpenGL/glu.h> // to be removed

@interface AppView : NSOpenGLView
{
    NSTimer*    animationTimer;
}
@end

@implementation AppView

-(void)prepareOpenGL
{
    [super prepareOpenGL];

#ifndef DEBUG
    GLint swapInterval = 1;
    [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
    if (swapInterval == 0)
        NSLog(@"Error: Cannot set swap interval.");
#endif
}

-(void)updateAndDrawDemoView
{
    [[self openGLContext] makeCurrentContext];
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Present
    [[self openGLContext] flushBuffer];

    if (!animationTimer)
        animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}

-(void)reshape                              { [super reshape]; [[self openGLContext] update]; [self updateAndDrawDemoView]; }
-(void)drawRect:(NSRect)bounds              { [self updateAndDrawDemoView]; }
-(void)animationTimerFired:(NSTimer*)timer  { [self setNeedsDisplay:YES]; }
-(void)dealloc                              { animationTimer = nil; }

@end


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
