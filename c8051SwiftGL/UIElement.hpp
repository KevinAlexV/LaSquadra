//
//  UIElement.hpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-28.
//
#ifndef UIElement_hpp
#define UIElement_hpp

#include <stdio.h>
#include "Drawable.hpp"
#include "Font support/GLESText.hpp"

class UIElement: public Drawable
{
public:
    UIElement(float, float, int);
    
    GLESText text;
    
    //char * font;
    
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
    float getXScale();
    float getYScale();
//    float getWidth();
//    float getHeight();
protected:
    float xScale, yScale;
private:
};

#endif
