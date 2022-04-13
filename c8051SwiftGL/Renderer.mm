//
//  Copyright © Borna Noureddin. All rights reserved.
//

#import "Renderer.h"
#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#include <chrono>
#include "GLESRenderer.hpp"

@interface Renderer () {
    GLKView *theView;               // to access some iOS-specific parameters needed to set up OpenGL
    GLESRenderer *glesRenderer;     // the OpenGL ES C++ render class
}

@end

@implementation Renderer

@synthesize isRotating; // each public property needs to be synthesized
@synthesize panX;
@synthesize panY;
@synthesize score;

- (void)dealloc
{
    delete glesRenderer;    // free up the memory of the GLESRenderer object
}

- (void)setup:(GLKView *)view
{
    // These lines of code set up the OpenGL context using Objective-C/iOS code
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!view.context) {
        NSLog(@"Failed to create ES context");
    }
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    theView = view;
    [EAGLContext setCurrentContext:view.context];

    // The next lines of code also use the Core Graphics library in iOS to read in a
    //  texture image so that it can be passed on to the C++ class.
    // This code could be moved into the C++ class, which would then need to
    //  load in the image (possibly using some other API/library).
    // Note in that case the need still to construct the path to the file in the iOS bundle
    //  just like in the case of the shader files (see below).
    vector<NSString*> textureNames;
    textureNames.push_back(@"dirt.jpg");
    textureNames.push_back(@"iron.jpg");
    textureNames.push_back(@"coin.jpg");
    textureNames.push_back(@"ExitPortal.png");
    textureNames.push_back(@"PotionEmpty.png");
    textureNames.push_back(@"PotionRed.png");
    textureNames.push_back(@"PotionOrange.png");
    textureNames.push_back(@"PotionYellow.png");
    textureNames.push_back(@"PotionGreen.png");
    textureNames.push_back(@"PotionBlue.png");
    textureNames.push_back(@"PotionDarkBlue.png");
    textureNames.push_back(@"PotionPurple.png");
    textureNames.push_back(@"PotionPink.png");
    vector<GLubyte*> textureDataList;
    vector<size_t> textureWidthList, textureHeightList;
    
    for(int i = 0; i < textureNames.size(); i++){
        CGImageRef textureImage = [UIImage imageNamed:textureNames[i]].CGImage;
        if (!textureImage) {
            NSLog(@"Failed to load image %@", textureNames[i]);
            exit(1);
        }
        //size_t textureWidth = CGImageGetWidth(textureImage);
        //size_t textureHeight = CGImageGetHeight(textureImage);
        textureWidthList.push_back(CGImageGetWidth(textureImage));
        textureHeightList.push_back(CGImageGetHeight(textureImage));
        GLubyte *textureData = (GLubyte *) calloc(textureWidthList[i]*textureHeightList[i]*4, sizeof(GLubyte));
        CGContextRef textureContext = CGBitmapContextCreate(textureData, textureWidthList[i], textureHeightList[i], 8, textureWidthList[i]*4, CGImageGetColorSpace(textureImage), kCGImageAlphaPremultipliedLast);
        CGContextDrawImage(textureContext, CGRectMake(0, 0, textureWidthList[i], textureHeightList[i]), textureImage);
        CGContextRelease(textureContext);
        textureDataList.push_back(textureData);
    }
  
    // Construct the C++ object, passing in the path to the shader files and the one texture data
    glesRenderer = new GLESRenderer([[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.vsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.vsh"] pathExtension]] cStringUsingEncoding:1],
                                    [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.fsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.fsh"] pathExtension]] cStringUsingEncoding:1],
                                    textureDataList.data(), textureWidthList.data(), textureHeightList.data());
    isRotating = glesRenderer->isRotating;
    panX = glesRenderer->panX;
    panY = glesRenderer->panY;
    score = 0;

    
    
    vector<NSString*> fontNames;
    fontNames.push_back(@"times");
    //fontNames.push_back(@"Vonique 64");
    //fontNames.push_back(@"arial");
    
    for(int i = 0; i < fontNames.size(); i++)
    {
        
        NSString *fontPath = [[NSBundle mainBundle] pathForResource:fontNames[i] ofType:@"ttf"];
        
        //if (!fontPath) {
          //  NSLog(@"Failed to load font %@", fontNames[i]);
        //}
        
        //glesRenderer->fonts.push_back((char *)[fontPath cStringUsingEncoding:NSUTF8StringEncoding]);
        
    }
    
    
    
    // Once we have passed on the data from the image file for the texture, we can free up the memory
    for(int i = 0; i < textureDataList.size(); i++)
        free(textureDataList[i]);
}

- (void)update
{
    glesRenderer->SetViewport(theView.drawableWidth, theView.drawableHeight);
    glesRenderer->isRotating = isRotating;
    glesRenderer->panX = panX;
    glesRenderer->panY = panY;
    glesRenderer->Update();
}

- (void)draw:(CGRect)drawRect;
{
    glesRenderer->Draw();
}

- (void)reset
{
    glesRenderer->reset();
}

- (bool)achievedGoal {
    return glesRenderer->achievedGoal();
}

- (float)getGameTime {
    return glesRenderer->getGameTime();
}

- (NSString*)getWinMsg{
    
    //return [NSString stringWithCString:glesRenderer->getWinMsg().c_str()
//encoding:[NSString defaultCStringEncoding]];
  
    return [NSString stringWithUTF8String:glesRenderer->getWinMsg().c_str()];

}

- (void)setPlayerDir:(int)playerDir;
{
    glesRenderer->setPlayerDir(playerDir);
}

- (void)handleDoubleTap:(float)xPos :(float)yPos :(float)sWidth : (float)sHeight
{
    
    glesRenderer->handleDoubleTap(xPos, yPos, sWidth, sHeight);
    
}

@end
