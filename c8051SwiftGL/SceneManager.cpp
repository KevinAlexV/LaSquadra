//
//  SceneManager.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#include "SceneManager.hpp"

void SceneManager::reset(){
    scene->reset();
}

void SceneManager::assignScene(SceneName name){
    curSceneName = name;
    switch(name){
        case MENU:
            //scene = new MenuScene();
            break;
        case MAZE:
            scene = new MazeScene();
            break;
        case CHEMISTRY_LAB:
            scene = new PotionScene();
            break;
        case OBSTACLE_COURSE:
            //scene = new ObstacleCourse();
            break;
    }
    
    scene->loadModels();
}

void SceneManager::pan(float panX, float panY){
    scene->pan(panX, panY);
}

void SceneManager::movePlayer(int playerDir) {
    scene->movePlayer(playerDir);
}

void SceneManager::update(){
    scene->update();
}

void SceneManager::draw(float aspect, GLint mvpMatrixUniform, GLint normalMatrixUniform){
    scene->draw(textureIds, aspect, mvpMatrixUniform, normalMatrixUniform);
}

void SceneManager::pushTexture(GLuint textureId){
    textureIds.push_back(textureId);
}

bool SceneManager::checkGoalCondition() {
    return scene->achievedGoal();
}

float SceneManager::getGameTime()
{
    return scene->getTimeLeft();
}

void SceneManager::setSceneFont(char * newFont){
    
}
