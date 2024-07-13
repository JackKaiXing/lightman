
#include "platformhandle.h"

#import <Cocoa/Cocoa.h>

// ----------------------------------------------------------------------------
// VIEW AND CONTEXT
// ----------------------------------------------------------------------------

using RenderCallback = std::function<void()>;
using SetupCallback = std::function<void()>;
using DestoryCallback = std::function<void()>;

@interface AppView : NSOpenGLView
{
    NSTimer* animationTimer;
    RenderCallback render;
}
@end

@implementation AppView

-(void)prepareOpenGL
{
    [super prepareOpenGL];
    //https://developer.apple.com/library/archive/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/EnablingOpenGLforHighResolution/EnablingOpenGLforHighResolution.html
    [self setWantsBestResolutionOpenGLSurface:YES];
}

-(void)updateView
{
    if (!animationTimer)
        animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}

-(void)setRenderCallback:(RenderCallback)fb
{
    render = fb;
}

// https://developer.apple.com/documentation/appkit/nsopenglview/1414948-reshape
-(void)reshape                              { [super reshape]; [self updateView]; }
// https://developer.apple.com/documentation/uikit/uiview/1622529-drawrect
-(void)drawRect:(NSRect)bounds              { [self updateView]; render(); }
-(void)animationTimerFired:(NSTimer*)timer  { [self setNeedsDisplay:YES]; }
-(void)dealloc                              { [super dealloc]; animationTimer = nil; }

@end

// ----------------------------------------------------------------------------
// WINDOW
// ----------------------------------------------------------------------------
@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, readonly) NSWindow* window;
@property (nonatomic, readwrite) RenderCallback render;
@property (nonatomic, readwrite) SetupCallback setup;
@property (nonatomic, readwrite) DestoryCallback destory;
@property (nonatomic, readwrite) NSSize windowSize;
@end

@implementation AppDelegate
@synthesize window = _window;

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    _destory();
    return YES;
}

-(NSWindow*)window
{
    if (_window != nil)
        return (_window);

    NSRect viewRect = NSMakeRect(100.0, 100.0, _windowSize.width, _windowSize.height);

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
    
    AppView* view = [[AppView alloc] initWithFrame:self.window.frame];
    [view setRenderCallback:_render];

    #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
        if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
            [view setWantsBestResolutionOpenGLSurface:YES];
    #endif // MAC_OS_X_VERSION_MAX_ALLOWED >= 1070

    [self.window setContentView:view];
    
    _setup();
}

-(void)setRenderCallback:(RenderCallback)fb
{
    _render = fb;
}

-(void)setSetupCallback:(SetupCallback)fb
{
    _setup = fb;
}

-(void)setDestoryCallback:(DestoryCallback)fb
{
    _destory = fb;
}

-(void)setWidowSize:(NSSize)size
{
    _windowSize = size;
}

@end

// ----------------------------------------------------------------------------
// RETRY INTERFACE
// ----------------------------------------------------------------------------
namespace lightmangui
{
    AppDelegate* delegate = nullptr;
    void* GetNativeWindow()
    {
        return [delegate.window contentView];
    }
    float GetBackScaleFactor()
    {
        // https://developer.apple.com/documentation/appkit/nswindow/1419459-backingscalefactor
        return [[delegate.window contentView] wantsBestResolutionOpenGLSurface]? [delegate.window backingScaleFactor] : 1.0;
    }
    int MainWindow(int argc, const char* argv[], 
        std::function<void()> setup,
        std::function<void()> render,
        std::function<void()> destory,
        unsigned int w, unsigned int h)
    {
        @autoreleasepool
        {
            NSApp = [NSApplication sharedApplication];
            delegate = [[AppDelegate alloc] init];
            [delegate setRenderCallback:render];
            [delegate setSetupCallback:setup];
            [delegate setDestoryCallback:destory];
            [delegate setWidowSize:NSMakeSize(w, h)];
            [[NSApplication sharedApplication] setDelegate:delegate];
            [NSApp run];
        }
        return NSApplicationMain(argc, argv);
    }
}
