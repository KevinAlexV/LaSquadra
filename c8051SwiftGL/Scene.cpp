//
//  Scene.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//
//If we want to make backdrops, try this https://learnopengl.com/Advanced-OpenGL/Cubemaps

#include "Scene.hpp"
#include "Global.h"
// _______________________   Scene Functions, a parent class to all 'scenes' in the game, as defined in scene.hpp ________________

Scene::Scene(){
    srand (time(NULL));
}

//Delete all drawables and the camera upon a deconstruction of this class.
Scene::~Scene(){
    for(Drawable *d : drawables)
        delete d;
    delete camera;
    delete playerDrawable;
    delete goal;
}

//Add a new 'drawable' element to the scene to be rendered upon a 'draw' call.
void Scene::addDrawable(Drawable *d){
    drawables.push_back(d);
}

//Reset scene by resetting the camera to it's default state, thus allowing the objects on screen to render in their default state, since the
//camera is reset.
void Scene::reset(){
    camera->getTransform()->setPosition(vec3(0.f, 0.f, 0.f));
    camera->getTransform()->setAngles(vec3(0.f, 0.f, 0.f));
    sceneGoalCondition = rand() % 2;
}

float Scene::normalize(float value, float min, float max) {
    return std::abs((value - min) / (max - min));
}


void Scene::pan(float panX, float panY){
    
}

void Scene::handleDoubleTap(float inputX, float inputY, float vpWidth, float vpHeight, float sWidth, float sHeight){
    
}

void Scene::movePlayer(int playerDir) {
    
}


//Get game timer for the scene
float Scene::getTimeLeft()
{
    return timeLeft;
    
}

//Update the transform when this scene is updated, and specify when the last frame was calculated (based on current time)
void Scene::update(){
    updateTransform();
    now = std::chrono::steady_clock::now();
    
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrame).count();
    
    if(duration >= 1.0 && gameStarted && timeLeft > 0.0f)
        timeLeft -= 1.0f;
    
    lastFrame = std::chrono::steady_clock::now();
}

//Update the transform of each drawable, which is specified in drawable's class definition.
void Scene::updateTransform(){
    for(Drawable* drawable : drawables)
        drawable->updateTransform();
}

//Draw current scene's elements, in reference to the MVP Camera position (the start position).
//Check scene's list of drawables, and for each drawable, get the indices, verts, etc, translate them in reference to the camera, so they are positioned based on where the camera is located.
void Scene::draw(vector<GLuint> textureIds, float aspect, GLint mvpMatrixUniform, GLint normalMatrixUniform){
    
    //Make the base mvp (model view projection) matrix equal to the identity matrix, translated by the camera position.Identity matrix is essentially a matrix with nothing in it.
    mvp = glm::translate(mat4(1.0f), camera->getTransform()->getPosition());
    mvpUI = glm::translate(mat4(1.0f), vec3(0.0f,0.0f,-2.0f));
    
    for(Drawable *drawable : drawables){
        int *indices = drawable->getIndices(), numIndices = drawable->getNumIndices();
        
        mat4 transform;
        
        if(drawable->isUI == true)
            transform = drawable->draw(mvpUI);
        else
            transform = drawable->draw(mvp);
        
        normalMatrix = glm::inverseTranspose(glm::mat3(transform));
        
        mat4 perspective = glm::perspective(60.0f * glm::pi<float>() / 180.f, aspect, 1.0f, 20.0f);
        vec3 angles = camera->getTransform()->getAngles();
        
        
        //If the drawable isnt a UI drawable, then rotate the drawable based on the camera's current perspective
        if(!drawable->isUI)
        {
            perspective = glm::rotate(perspective, radians(angles.x), vec3(1, 0, 0));
            perspective = glm::rotate(perspective, radians(angles.y), vec3(0, 1, 0));
            perspective = glm::rotate(perspective, radians(angles.z), vec3(0, 0, 1));
        }
        
        transform = perspective * transform;

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, value_ptr(transform));
        glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        
        glBindTexture(GL_TEXTURE_2D, textureIds[drawable->getTextureListIndex()]);
        //glUniform1i(textureUniform, textureIds[drawable->getTextureListIndex()]);
        
        glDrawElements ( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices );
    }
}

//Load models based on camera instance. The camera would be reset it to it's default state before drawables are rendered.
void Scene::loadModels(){
    camera = Camera::GetInstance();
}



// __________________________________________________________ Maze Scene _______________________________________________________________

void MazeScene::reset(){
    Scene::reset();
    camera->getTransform()->setAngles(vec3(35.f, 0.f, 0.f));
    if(drawables.size() > 4){
        playerDrawable->anim->setEnabled(false);
        Transform* transformSpeed = new Transform();
        playerDrawable->anim->assignTransformSpeed(transformSpeed);
        coinDrawables.clear();
        while(drawables.size() > 4)
            drawables.pop_back();
    }
    sceneWon = false;
    gameStarted = false;
    
    //cout << "Goal condition: " << sceneGoalCondition << endl;
    Maze* maze = new Maze(WALL_NUM);//random maze size
    
    //Print maze generation
    //maze->print();
    
    
    Drawable* back = new UIElement(25.0f, 12.5f, -10.0f, 13);
    addDrawable(back);
    
    bool goalNotAdded = true;
    float sector = 2.f / WALL_NUM;
    
    for(int r = 0; r < WALL_NUM; r++){
        for(int c = 0; c < WALL_NUM; c++){
            float centerX = -2.f + 2 * sector * (c + 1) - sector;
            float centerY = -2.f + 2 * sector * (r + 1) - sector;
            
            if(maze->maze[r * WALL_NUM + c].getWallVisible(1))
                addWall(false, centerX + sector, centerY, sector);
            if(maze->maze[r * WALL_NUM + c].getWallVisible(2))
                addWall(true, centerX, centerY + sector, sector);
            
            if(sceneGoalCondition == 0)
            {
                winConditionMsg = "Collect the Coins!";
                bool coinExists = rand() % 12 == 0; // coin generator
                if (coinExists) {
                    addCoin(centerX, centerY, sector / 2, 0.015, 2);
                }
            
            }//goal condition 1, render goal
            else if (sceneGoalCondition == 1 && goalNotAdded && ((r == (int)WALL_NUM/2) && (c == (int)WALL_NUM/2)))
            {
                winConditionMsg = "Get to the Goal!";
                addGoal((WALL_NUM - 1) * sector, -(WALL_NUM - 1) * sector, sector/2, 0.01, 3);
                goalNotAdded = false;
            }
        }
    }
    
    vec3 groundPos = drawables[1]->globalTransform->getPosition();
    playerDrawable->globalTransform->setPosition(vec3(groundPos.x + (float)WALL_NUM * sector - sector, groundPos.y + 0.5f, groundPos.z + (float)WALL_NUM * sector - sector));
    
    timeLeft = 750.0f;
    gameStarted = true;
}

// ------- Add drawables to scene ----------
//Add new wall drawable to the maze.
void MazeScene::addWall(bool horizontal, float posX, float posY, float alternateScale, int textureListIndex){
    addDrawable(new Cube(1));
    int lindex = drawables.size() - 1;
    vec3 groundPos = drawables[1]->globalTransform->getPosition();
    drawables[lindex]->globalTransform->setPosition(glm::vec3(groundPos.x + posX, groundPos.y + 0.5f, groundPos.z + posY));
    if(horizontal)
        drawables[lindex]->globalTransform->setScale(glm::vec3(alternateScale, 0.25f, 0.01f));
    else
        drawables[lindex]->globalTransform->setScale(glm::vec3(0.01f, 0.25f, alternateScale));
}

//Add new coin to the maze.
void MazeScene::addCoin(float posX, float posY, float radius, float thickness, int textureListIndex, int sectorCount){
    Drawable *coinDrawable = new Cylinder(textureListIndex, radius, thickness, sectorCount);
    addDrawable(coinDrawable);
    coinDrawables.push_back(coinDrawable);
    
    vec3 groundPos = drawables[1]->globalTransform->getPosition();
    coinDrawable->globalTransform->setPosition(vec3(groundPos.x + posX, groundPos.y + 0.5f, groundPos.z + posY));
    Transform* transformSpeed = new Transform();
    transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
    transformSpeed->setAngles(vec3(0, 5.f, 0.f));
    coinDrawable->assignAnimator(new Animator(transformSpeed));
    coinDrawable->anim->assignTransform(coinDrawable->localTransform);
    coinDrawable->anim->setEnabled(true);
}

//Add new goal object to the maze.
void MazeScene::addGoal(float posX, float posY, float radius, float thickness, int textureListIndex, int sectorCount){
    Drawable *portalDrawable = new Cylinder(textureListIndex, radius, thickness, sectorCount);
    addDrawable(portalDrawable);
    //coinDrawables.push_back(portalDrawable);
    
    goal = portalDrawable;
    
    vec3 groundPos = drawables[1]->globalTransform->getPosition();
    portalDrawable->globalTransform->setPosition(vec3(groundPos.x + posX, groundPos.y + 0.5f, groundPos.z + posY));
    //Transform* transformSpeed = new Transform();
    //transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
    //transformSpeed->setAngles(vec3(0, 0.1f, 0.1f));
    //portalDrawable->assignAnimator(new Animator(transformSpeed));
    //portalDrawable->anim->setEnabled(true);
}

//Add new UI Timer to the maze
void MazeScene::addTimer(float posX, float posY, int textureListIndex)
{
    //Add new drawable with texture element
    //addDrawable(new UIElement(0.25f, 0.15f, textureListIndex));
    
    //int lindex = drawables.size() - 1;
    //drawables[lindex]->globalTransform->setPosition(glm::vec3(posX, posY, 0.0f));
}

// -------- Load all drawables from scene (initializer) ------

//Load all maze models into the scnee.
void MazeScene::loadModels(){
    Scene::loadModels();
    
    //playerDrawable = new Sphere(1, 0.15f, 10, 10);
    playerDrawable = new Character(14, 0.15f);
    addDrawable(playerDrawable);
    
    Transform* transformSpeed = new Transform();
    //transformSpeed->setPosition(vec3(0.f, 0.f, 0.f));
    transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
    //transformSpeed->setAngles(vec3(0, 5.f, 5.f));
    transformSpeed->setAngles(vec3(0, 5.f, 0.f));
    playerDrawable->assignAnimator(new Animator(transformSpeed));
    playerDrawable->anim->assignTransform(playerDrawable->globalTransform);
    playerDrawable->anim->setBuildupSpeed(25.f);
    playerDrawable->anim->setEnabled(true);
    
    addDrawable(new Cube(0));
    drawables[1]->globalTransform->setScale(vec3(2.f, 0.25f, 2.f));
    drawables[1]->globalTransform->setPosition(vec3(0.f, -3.f, -3.5f));
    
    //When text is working, add a timer to the screen and render text to it.
    //addTimer(0.0f,1.0f,3);
    float wallNum = 8;
    float sector = 2.f / wallNum;
    addWall(true, 0.f, -2.f, 2.f);
    addWall(false, -2.f, sector, -2.f + sector);
    
    reset();
    /*enemy = new MazeEnemy();
    Drawable* enemyDrawable = new Cube(0);
    enemyDrawable->globalTransform->setPosition(vec3(-sector * wallNum / 2, 0.15f, -sector * wallNum / 2));
    drawables.push_back(enemyDrawable);*/
    //Drawable* enemyDrawable = new Cube(4);
    //enemyDrawable->globalTransform->setScale(vec3(0.1, 0.1, 0.1));
    //enemyDrawable->globalTransform->setPosition(vec3(0.f, 1.f, 0.f));
    //enemyDrawable->globalTransform->setPosition(vec3(0.f, -2.5f, -3.5f));
    //addDrawable(enemyDrawable);
}

void MazeScene::update(){
    Scene::update();
    if(playerDrawable->anim->isMoving()){
        vec3 playerPos = playerDrawable->globalTransform->getPosition();
        for (int i = 0; i < coinDrawables.size(); i++) {
            Drawable *drawable = coinDrawables[i];
            vec3 position = drawable->globalTransform->getPosition();
            float deltaX = position.x - playerPos.x;
            float deltaY = position.z - playerPos.z;
            float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < 0.2) {
                // remove collide coin
                coinDrawables.erase(coinDrawables.begin() + i);
                remove(drawables.begin(), drawables.end(), drawable);
                AudioManager::playCoinCollectedAudio();
            }
        }
    }
}

// ---------- Other scene specific functions -------------


void MazeScene::pan(float panX, float panY)
{
    auto curTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(curTime - lastFrame).count();
    float ratio = elapsedTime * 0.000005f;
    
    if(abs(panY) >= abs(panX))
    {
        camera->getTransform()->translate(vec3(0, 0, panY * ratio));
    }
    else
    {
        float hyp = sqrt(pow(panX, 2) + pow(panY, 2));
        float sin = panX / hyp;
        camera->getTransform()->rotate(vec3(0, asin(-sin), 0));
    }
}

//Translate ball to x,y instead of current x,y position.
void MazeScene::movePlayer(int playerDir) {
    //cout << "Player Dir: " << playerDir + "\n";
    
    this->playerDir = playerDir;
    if (!playerDrawable) {
        return;
    }
    
    Transform* transformSpeed = playerDrawable->anim->getTransformSpeed();
    float speed = 0.1f;
    bool enabled = true;
    switch(playerDir){
        case -1:
            enabled = false;
            break;
        case 0:
            transformSpeed->setPosition(vec3(0.f, 0.f, -speed));
            break;
        case 1:
            transformSpeed->setPosition(vec3(speed, 0.f, 0.f));
            break;
        case 2:
            transformSpeed->setPosition(vec3(0.f, 0.f, speed));
            break;
        case 3:
            transformSpeed->setPosition(vec3(-speed, 0.f, 0.f));
            break;
    }
    playerDrawable->anim->assignTransformSpeed(transformSpeed);
    playerDrawable->anim->setEnabled(enabled);
    
    vec3 playerPos = playerDrawable->globalTransform->getPosition();
    
    //collisionCheck(playerPos.x, playerPos.z);
    // check collision for coins, if not all coins are collected.
    if(!isAllCoinsCollected())
    {
        for (int i = 0; i < coinDrawables.size(); i++)
        {
            Drawable *drawable = coinDrawables[i];
            vec3 position = drawable->globalTransform->getPosition();
            float deltaX = position.x - playerPos.x;
            float deltaY = position.z - playerPos.z;
            float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < 0.2) {
                // remove collide coin
                coinDrawables.erase(coinDrawables.begin() + i);
                remove(drawables.begin(), drawables.end(), drawable);
                AudioManager::playCoinCollectedAudio();
            }
        }
    }
    
    //If the player should be looking for the goal, check if the goal has been collided with.
    if(sceneGoalCondition == 1)
    {
        vec3 position = goal->globalTransform->getPosition();
        
        //cout << "\nPlayer Y: " << playerPos.y;
        
        float deltaX = position.x - playerPos.x;
        float deltaY = position.z - playerPos.z;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        
        //cout << distance << "\nX: " << deltaX <<"\nY: "<< deltaY;
        if (distance < 0.2) {
            cout << "goal found!";
            sceneWon = true;
        }
    }
}

int MazeScene::collisionCheck(float posX, float posY)
{
    int collision = 0;
    int row = floor(posX * 2 + 4);
    int column = floor(posY * 2 + 4);
    
    float deltaX = (row * (MAZE_CELL_SIZE / 2.0f)) - posX;
    float deltaY = (column * (MAZE_CELL_SIZE / 2.0f)) - posY;
    
    collision = wallCheck(row, column, posX, posY);
    if(deltaX > 0 && row < maze->getSize())
        collision &= wallCheck(row - 1, column, posX, posY);
    else if(deltaX < 0 && row > 0)
        collision &= wallCheck(row + 1, column, posX, posY);
    if(deltaY > 0 && column < maze->getSize())
        collision &= wallCheck(row, column - 1, posX, posY);
    else if(deltaY < 0 && column > 0)
        collision &= wallCheck(row, column + 1, posX, posY);
    
    return collision;
}

int MazeScene::wallCheck(int row, int column, float posX, float posY)
{
    int collision = 0;
    /*int topC = column > 0 ? column - 1 : column;
    int leftR = row > 0 ? row - 1 : row;
    
    float cellCenterX = row * 0.5 - 1.75;
    float cellCenterY = column * 0.5 - 1.75;
    
    float deltaX = posX - cellCenterX;
    float deltaY = posY - cellCenterY;
    
    MazeSector curCell = maze->getSector(row, column);
    MazeSector topCell = maze->getSector(row, topC);
    MazeSector leftCell = maze->getSector(leftR, column);
    
    switch(curCell.getType())
    {
        case 0:
            if(!curCell.getWallHidden(0))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(1))
            {
                float check = abs(deltaX + PLAYER_RADIUS + SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(2))
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(3))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            break;
        case 1:
            if(!curCell.getWallHidden(0))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(1))
            {
                float check = abs(deltaX + PLAYER_RADIUS + SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(2))
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!leftCell.getWallHidden(1))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            break;
        case 2:
            if(!curCell.getWallHidden(0))
            {
                float check = deltaX + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(1))
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(2))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!topCell.getWallHidden(1))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            break;
        case 3:
            if(!curCell.getWallHidden(0) && deltaX > 0)
            {
                float check = deltaX + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(1) && deltaY < 0)
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!topCell.getWallHidden(1))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!leftCell.getWallHidden(1))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            break;
    }*/
    
    return collision;
}

bool MazeScene::isAllCoinsCollected() {
    return coinDrawables.empty();
}

bool MazeScene::achievedGoal()
{
   switch(sceneGoalCondition)
   {
    //Collect the coins!
       case 0:
           if(coinDrawables.empty())
           {
               cout << "All coins collected!";
               gameStarted = false;
               sceneWon = true;
           }
           return sceneWon;
           break;
       //Escape the maze!
       case 1:
           if (sceneWon)
               gameStarted = false;
           return sceneWon;
           //cout << "You win! But not really because this needs to be set once collisions are in.";
           break;
       default:
           cout<<"Error: MazeScene does not have a goalCondition set. Please check Scene.cpp, as the current condition is " << sceneGoalCondition;
           break;
   }
    
    return sceneWon;
}




// __________________________________________________________ Potion Scene _______________________________________________________________



void PotionScene::reset(){
    Scene::reset();
    
    potionsNeeded.clear();
    sceneWon = false;
    gameStarted = false;
    
    sceneGoalCondition = 0;
    
    cout << "Goal condition: " << sceneGoalCondition << endl;
    
    bool goalNotAdded = true;
    
    //How many potions in total should be created
    int potionsToCreate = 8;
    //How many potions should be made per row, and the calculated column as a result of the row and toCreate vals
    potionsPerRow = 4;
    potionsPerColumn = potionsToCreate/potionsPerRow;
    
    string randColor = "";
    
    Drawable* back = new UIElement(25.0f, 12.5f, -10.0f, 13);
    addDrawable(back);
    
    //Determine win condition for potion scene. (currently, 2 can be generated)
    switch(sceneGoalCondition)
    {
        case 0:
            randColor = getRandomColor();
            potionsNeeded.push_back(randColor);
            winConditionMsg = "Pick the " + randColor + " potion!";
            break;
        case 1:
            //randColor = getRandomColor();
            //potionsNeeded.push_back(randColor);
            //winConditionMsg = "Pick the " + randColor + " and ";
            //randColor = getRandomColor();
            //potionsNeeded.push_back(randColor);
            //winConditionMsg += randColor + " potion!";
            break;
        default:
            break;
    }
    
    //For each potion per column, go through each row (so if only 2 potions can be created per column, go through 2 rows of potions/rendering)
    for(int i = 0; i < potionsPerColumn; i++)
    {
        //For each potion that can be created/rendered per row, render/create a new potion.
        for(int j = 0; j<potionsPerRow; j++)
        {
            //std::cout<<"Creating potion"<<endl;
            
            //Create a potion starting at trasnform -.5f, and spread them out based on i and j. The texture starts at the potionStartingTexture (saved in our backend), and counts up.
            addPotion(-.5f + j*.3f, 0.0f + i*.3f,(potionStartingTexture+1)+j+(potionsPerRow*i));
        }
    }
    cout << winConditionMsg << endl << endl;
    //Set the amount of time this scene will have, and start the game.
    timeLeft = 500.0f;
    gameStarted = true;
}


void PotionScene::loadModels(){
    Scene::loadModels();
    
    reset();
}

void PotionScene::update(){
    Scene::update();
}

string PotionScene::getRandomColor()
{
    
    int pickedColor = rand() % 8;
    
    switch(pickedColor)
    {
        case 0:
            return "Red";
            break;
        case 1:
            return "Orange";
            break;
        case 2:
            return "Yellow";
            break;
        case 3:
            return "Green";
            break;
        case 4:
            return "Blue";
            break;
        case 5:
            return "DarkBlue";
            break;
        case 6:
            return "Purple";
            break;
        case 7:
            return "Pink";
            break;
        default:
            return "";
            break;
    }
}


void PotionScene::movePlayer(int direction)
{
    
    int startAtValue = 0;
    
    
    //Switch the direction given by the buttons in viewcontroller and add/subtract from the selection x/y value.
    switch(direction){
        case -1:
            break;
        case 0:
            selected.y = selected.y + 1;
            break;
        case 1:
            selected.x = selected.x + 1;
            break;
        case 2:
            selected.y = selected.y - 1;
            break;
        case 3:
            selected.x = selected.x - 1;
            break;
    }
    
    
    //If the selected x/y is less than 0, or greater than the potionsPer Row/Column (out of array scope), then set it to the min/max.
    if(selected.x<0)
        selected.x = 0;
    
    if(selected.x > (potionsPerRow - 1))
        selected.x = (potionsPerRow - 1);
    
    if(selected.y > (potionsPerColumn-1))
        selected.y = (potionsPerColumn-1);
    
    if(selected.y<0)
        selected.y = 0;
    
    
    
    //If the selected y is greater than or equal to one, then multiply the selected.y by the potions per row to make this selectin into a 1d array element selectin
    //(Since selected is 2d, and the potions array is 1d, just want to convert it. In terms of selection, Potions array starts at 0,0 then 0,1 -> 0,2 -> 0,3 -> 1,0 -> 1,1 and so on)
    //So since we want to go by the rows, we simply say 'if we're on the next row, rep by y, then multiple y by the amount per row, and start from there).
    if(selected.y >= 1)
        startAtValue = selected.y * potionsPerRow;
    
    
    //Make the selection to the startAtValue (based on what row we're on), plus the selected.x (the column we're on)
    selection = startAtValue + selected.x;
    
    //If the direction isnt -1, which is the 'release' of the button, then highlight that potion
    if(direction != -1)
    {
        potions[selection]->highlight();
        
        if(prevSelection != -1)
            potions[prevSelection]->highlight();
    }
    //Make previous selectin current selection so we can unhighlight the previous potion
    prevSelection = selection;
    
    //potions.erase(potions.begin() + selection);
    //remove(drawables.begin(), drawables.end(), potions[selection]->potion);
    
    //cout<< "x: " << selected.x << " y: " << selected.y<<endl;
}

void PotionScene::handleDoubleTap(float inputX, float inputY, float vpWidth, float vpHeight, float sWidth, float sHeight){
    
    //If a selectin has been made, and the game has not just started.
    if(selection != -1)
    {
        potions[selection]->select();
        
        int haveToWin = 0;
        
        for(int i = 0; i < potionsNeeded.size(); i++)
        {
            if(potions[selection]->compareColor(potionsNeeded[i]))
                haveToWin ++;
        }
        
        if(haveToWin >= potionsNeeded.size())
        {
            sceneWon = true;
        }
        cout << "Scene won? " << sceneWon << endl;
    }
    // ************************************************************ NOTE *********************************************************************
    //The code below was used to try and convert a 'tap' gesture from swift into OpenGL's coordinate system (our transforms).
    //This was an unsuccessful attempt to do so, but I'll (Kevin) leave it here for future reference in case we decide to reattempt the logic.
    
    //VPWidth/height = (640, 1136), sWidth/Height = (320, 568)
    
    //Note that from swift, the input values are placed on a grid where 0,0 is the bottom left of the screen.
    //cout << "True X: " << inputX <<endl<< "True Y: " << inputY << endl;

    /*
    //Assuming input starts at the top left, at 0 0, translate it so it starts at the bottom left instead. 600 is the height, 800 width?
    inputX = inputX;
    inputY = inputY*-1 + sHeight;
    
    
    //Normalize the coordinates to get smaller numbers without losing distance data (essentially a percentage of the width/height provided). Should now be comparable to potion transforms.
    inputX = normalize(inputX, 0, sWidth);
    inputY = normalize(inputY, 0, sHeight);
    
    //Wx (tap position) is Vx (center of screen), smin/max (tap canvas, where a valid tap is registerable), Xmin/max (position of object) Vx-Smin/sMAX - sMin    *     (Xmax-Xmin) + Xmin
    cout << "\n\n\nTap X: " << inputX << endl <<"Tap Y: " << inputY <<endl;
    
    //perspective matrix = viewMatrix. This is the translation matrix, and pos of the camera
    glm::mat4x4 viewMat = glm::translate(mat4(1.0f), vec3(0.0f,0.0f,-2.0f));
    
    //Create perspective/projection matrix. Angle rotatin from camera, 	window size/aspect ration, short clip distance and far clip distance (before object no longer renders)
    mat4 perspective = glm::perspective(60.0f * glm::pi<float>() / 180.f, vpWidth / vpHeight, 1.0f, 20.0f);
    
    
    for (int i = 0; i < potionDrawables.size(); i++)
    {
        Drawable *drawable = potionDrawables[i];
        vec3 position = drawable->globalTransform->getPosition();
        
        //Convert World coords to 2d screen coords
        //Remember: OpenGL's window-space is relative to the bottom-left of the window, not the top-left. the 1.0 in vec4 is scale (supposedly)
        vec4 clipSpacePos = perspective * (viewMat * vec4(position, 1.0));
        vec2 windowSpacePos;
        
        //cout << "clipspace X: " << clipSpacePos.x << "\nclipspace Y: " << clipSpacePos.y << "\nclipspace Z: " << clipSpacePos.z << endl;

        if(clipSpacePos.w != 0)
        {
            vec3 ndcSpacePos;
            //Calculate scaled coordinate.
            ndcSpacePos.x = clipSpacePos.x / clipSpacePos.w;
            ndcSpacePos.y = clipSpacePos.y / clipSpacePos.w;
            ndcSpacePos.z = clipSpacePos.z / clipSpacePos.w;
            
            //Calculate coordinate respective to window
            windowSpacePos.x = (((ndcSpacePos.x + 1.0) / 2.0) * vpWidth);
            windowSpacePos.y = (((ndcSpacePos.y + 1.0) / 2.0) * vpHeight);
            
            //windowSpacePos.x = normalize(windowSpacePos.x, 0, 1);
            //windowSpacePos.y = normalize(windowSpacePos.y, 0, 1);
            
            if(i != -1)
            {
                //cout<< "ObjectN X: " << ndcSpacePos.x << endl <<"ObjectN Y: " << ndsSpacePos.y <<endl << endl;
                cout<<"Object X: " << windowSpacePos.x << endl <<"Object Y: " << windowSpacePos.y << endl <<endl << endl;
            
            }
        }
        
        //cout<< "X: " << inputX << endl <<"Y: " << inputY <<endl;
        //cout<< "Object X: " << position.x << endl <<"Object Y: " << position.y <<endl << endl;
        
        float deltaX = windowSpacePos.x/vpWidth - inputX;
        float deltaY = windowSpacePos.y/vpHeight - inputY;
        
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        
        //cout << "DeltaX: " <<  deltaX << endl << "DeltaY: " << deltaY << endl;
        //cout << "Distance: " << distance << endl;
        
        //width
        //float width = vertices[2] - vertices[1];
        
        //Height
        //vertices[2] - vertices[0]
        
        //cout << drawable->getWidth();
        
        if (distance < .2) {
            
            //potionDrawables.erase(potionDrawables.begin() + i);
            remove(drawables.begin(), drawables.end(), drawable);
        }
    }*/
}

//void movePlayer(int) override;
bool PotionScene::achievedGoal()
{
    switch(sceneGoalCondition)
    {
        //Select specific potion!
        case 0:
            if(sceneWon)
            {
                cout << "Potion found!" << endl;
                gameStarted = false;
            }
            return sceneWon;
            break;
        //Select Specific potions!
        case 1:
            if (sceneWon)
            {
                cout << "Potions found" << endl;
                gameStarted = false;
            }
            return sceneWon;
            break;
        default:
            cout<<"Error: PotionScene does not have a goalCondition set. Please check Scene.cpp, as the current condition is " << sceneGoalCondition;
            break;
    }
    
    return sceneWon;
}


//-------------------------- Renderable Objects -----------------------------

//Add new UI Timer to the maze
void PotionScene::addPotion(float posX, float posY, int textureListIndex)
{
    //Add new drawable with texture element
    Potion *potion = new Potion(posX, posY, textureListIndex, 4);
    potions.push_back(potion);
    
    addDrawable(potion->potion);
    addDrawable(potion->potionHighlight);
    
    //std::cout<< "Potion X: " << posX << endl << "Potion Y: " << posY << endl;// << "Potion Z: "<< endl;
}
