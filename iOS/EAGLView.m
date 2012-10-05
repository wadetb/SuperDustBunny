 //
//  EAGLView.m
//  OpenGLES_iPhone
//
//  Created by mmalc Crawford on 11/18/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"

extern int _msNewX;
extern int _msNewY;
extern int _msNewButton1;

extern float _msNewAccelX;
extern float _msNewAccelY;
extern float _msNewAccelZ;

extern GLuint _gxDefaultFrameBuffer;
extern GLuint _gxDefaultFrameBufferWidth;
extern GLuint _gxDefaultFrameBufferHeight;

extern float LitScreenHeight;

void GetInput_BeginSwipe(float X, float Y, double Time);
void GetInput_AddToSwipe(float X, float Y, double Time);
void GetInput_EndSwipe(float X, float Y, double Time);

@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView

@synthesize context;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
        
        currentTouch = NULL;
    }
    
    return self;
}

- (void)dealloc
{
    [self deleteFramebuffer];    
    [context release];
    
    [super dealloc];
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext) {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer) {
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
      
        _gxDefaultFrameBuffer = defaultFramebuffer;
        _gxDefaultFrameBufferWidth = framebufferWidth;
        _gxDefaultFrameBufferHeight = framebufferHeight;
    }
}

- (void)deleteFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer) {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer) {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
    }
}

- (void)setFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	// This applies a pretty heavy smoothing function to the accelerometer.
	accel[0] = acceleration.x * 0.5f + accel[0] * (1.0f - 0.5f);
	accel[1] = acceleration.y * 0.5f + accel[1] * (1.0f - 0.5f);
	accel[2] = acceleration.z * 0.5f + accel[2] * (1.0f - 0.5f);
	_msNewAccelX = accel[0];
	_msNewAccelY = accel[1];
	_msNewAccelZ = accel[2];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (currentTouch)
        return;
    
	currentTouch = [touches anyObject];
    
    if (currentTouch) {
        CGPoint touchPoint = [currentTouch locationInView:self];
        _msNewX = touchPoint.x * 768.0f / framebufferWidth;
        _msNewY = touchPoint.y * LitScreenHeight / framebufferHeight;
        _msNewButton1 = 1;
        
        GetInput_BeginSwipe(_msNewX, _msNewY, event.timestamp);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (!currentTouch)
        return;

    if (![touches containsObject:currentTouch])
        return;
    
    if (currentTouch) {
        CGPoint touchPoint = [currentTouch locationInView:self];
        _msNewX = touchPoint.x * 768.0f / framebufferWidth;
        _msNewY = touchPoint.y * LitScreenHeight / framebufferHeight;
        
        GetInput_AddToSwipe(_msNewX, _msNewY, event.timestamp);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (!currentTouch)
        return;

    if (![touches containsObject:currentTouch])
        return;
    
    if (currentTouch) {
        CGPoint touchPoint = [currentTouch locationInView:self];
        _msNewX = touchPoint.x * 768.0f / framebufferWidth;
        _msNewY = touchPoint.y * LitScreenHeight / framebufferHeight;
        _msNewButton1 = 0;
        
        GetInput_EndSwipe(_msNewX, _msNewY, event.timestamp);
        
        currentTouch = NULL;
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (!currentTouch)
        return;
    
    if (![touches containsObject:currentTouch])
        return;
    
    if (currentTouch) {
        CGPoint touchPoint = [currentTouch locationInView:self];
        _msNewX = touchPoint.x * 768.0f / framebufferWidth;
        _msNewY = touchPoint.y * LitScreenHeight / framebufferHeight;
        _msNewButton1 = 0;
        
        GetInput_EndSwipe(_msNewX, _msNewY, event.timestamp);
        
        currentTouch = NULL;
    }    
}


@end

