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
    selectTexture = selectedcolor;
    
    switch(currentTexture)
    {
        case 5:
            colorName = "Red";
            color = 1;
            break;
        case 6:
            colorName = "Orange";
            color = 2;
            break;
        case 7:
            colorName = "Yellow";
            color = 3;
            break;
        case 8:
            colorName = "Green";
            color = 4;
            break;
        case 9:
            colorName = "Blue";
            color = 5;
            break;
        case 10:
            colorName = "DarkBlue";
            color = 6;
            break;
        case 11:
            colorName = "Purple";
            color = 7;
            break;
        case 12:
            colorName = "Pink";
            color = 8;
            break;
        default:
            colorName = "";
            color = 0;
            break;
    }
    
    potion->globalTransform->setPosition(glm::vec3(posX, posY, potion->globalTransform->getPosition().z));
    potionHighlight->globalTransform->setPosition(glm::vec3(posX, posY, potionHighlight->globalTransform->getPosition().z));
    //std::cout<< "Potion X: " << posX << endl << "Potion Y: " << posY << endl;// << "Potion Z: "<< endl;
    
}

Potion::~Potion()
{
    delete potion;
    delete potionHighlight;
}

void Potion::select()
{
    selected = !selected;
    
    //If selected, change texture to 'empty' bottle texture, saved at text index 4 as of writing. Texture will be based on fed 'selectedcolor' val from constructor, stored in selectTexture variable.
    if(selected)
        potion->changeTexture(selectTexture);
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

bool Potion::compareColor(string compColor)
{
    if(compColor == colorName)
        return true;
    else
        return false;
}
