//
//  Character.hpp
//  c8051SwiftGL
//

#ifndef Character_hpp
#define Character_hpp

#include <stdio.h>
#include "Drawable.hpp"

class Character : public Drawable {
    float radius;
    void loadAll();
    
public:
    Character(int, float);
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
};

#endif /* Character_hpp */
