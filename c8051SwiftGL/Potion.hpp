//
//  Potion.hpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-04-12.
//

#ifndef Potion_hpp
#define Potion_hpp

#include <stdio.h>
#include "UIElement.hpp"

class Potion{
    
public:
    Drawable* potion;
    Drawable* potionHighlight;
    Potion(float, float, int, int);
    ~Potion();
    void select();
    void highlight();
    bool compareColor(string);
    
    //If the potion is selected, and if this potion is needed to solve the game.
    bool selected = false, neededToSolve = false, highlighted = false;
    
    //In order of color# - Empty, Red, Orange, Yellow, Green, Blue, Purple, Pink. Max of 8 colors/types
    int color = 0;
    string colorName = "Empty";
    
private:
    int currentTexture, selectTexture;
};

#endif /* Potion_hpp */
