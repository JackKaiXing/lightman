
#include "platformhandle.h"

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h> // TODO tobe removed
#import <OpenGL/glu.h> // TODO tobe removed

// ----------------------------------------------------------------------------
// VIEW AND CONTEXT
// ----------------------------------------------------------------------------

using RenderCallback = std::function<void()>;
using SetupCallback = std::function<void()>;

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
    int MainWindow(int argc, const char* argv[], std::function<void()> setup, std::function<void()> render)
    {
        @autoreleasepool
        {
            NSApp = [NSApplication sharedApplication];
            delegate = [[AppDelegate alloc] init];
            [delegate setRenderCallback:render];
            [delegate setSetupCallback:setup];
            [[NSApplication sharedApplication] setDelegate:delegate];
            [NSApp run];
        }
        return NSApplicationMain(argc, argv);
    }
}
