//
//  Copyright © Borna Noureddin. All rights reserved.
//  This is a minimal wrapper class for the OpenGL ES C++ render class (GLESRenderer).
//  It is used to bridge between Swift and C++.
//

#ifndef Renderer_h
#define Renderer_h
#import <GLKit/GLKit.h>

@interface Renderer : NSObject

@property bool isRotating;  // example of variable that can be passed from Swift to C++ code
@property float panX;
@property float panY;
@property int score;

- (void)setup:(GLKView *)view;  // do some basic GL setup
- (void)update;                 // wrapper for C++ update function
- (void)draw:(CGRect)drawRect;  // wrapper for C++ draw function
- (void)reset;                  // wrapper for C++ reset function
- (bool)achievedGoal;
- (float)getGameTime;
- (NSString*)getWinMsg;
- (void)setPlayerDir:(int)playerDir;
- (void)handleDoubleTap:(float)xPos:(float)yPos:(float)sWidth:(float)sHeight;

@end

#endif /* Renderer_h */
