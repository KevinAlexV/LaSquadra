//
//  Potion.cpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-04-12.
//

#include "Potion.hpp"

Potion::Potion(float posX, float posY, int textureListIndex, int selectedcolor)
{
    //Add new drawable with texture element
    potion = new UIElement(0.1f, 0.1f, textureListIndex);
    potionHighlight = new UIElement(0.11f, 0.11f, 3);
    
    currentTexture = textureListIndex;
    color = selectedcolor;
    
    potion->globalTransform->setPosition(glm::vec3(posX, posY, potion->globalTransform->getPosition().z));
    potionHighlight->globalTransform->setPosition(glm::vec3(posX, posY, potionHighlight->globalTransform->getPosition().z));
    //std::cout<< "Potion X: " << posX << endl << "Potion Y: " << posY << endl;// << "Potion Z: "<< endl;
    
}

void Potion::select()
{
    selected = !selected;

    if(selected)
        potion->changeTexture(4);
    else
        potion->changeTexture(currentTexture);
}

void Potion::highlight()
{
    highlighted = !highlighted;

    if(highlighted)
        potionHighlight->changeTexture(2);
    else
        potionHighlight->changeTexture(3);
}
