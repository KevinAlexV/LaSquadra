//
//  Maze.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-07.
//

#include "Maze.hpp"

MazeSector::MazeSector() {
    for(int i = 0; i < 4; i++)
        walls[i] = true;
}

void MazeSector::setWallVisible(int wall, bool isVisible){
    walls[wall] = isVisible;
}

bool MazeSector::getWallVisible(int wall){
    return walls[wall];
}

void MazeSector::print(){
    string s = "";
    if(walls[2])
        s += "_";
    else
        s += " ";
    if(walls[1])
        s += "|";
    else
        s += " ";
    cout << s;
}

Maze::Maze(int size){
    if(size < 2)
        throw 30;
    
    this->size = size;
    int row = 0;
    while(row < size){
        float col = 0;
        while(col < size){
            maze.push_back(MazeSector());
            col++;
        }
        row++;
    }
    load();
}

//On unvisited cells, generate cell.
int Maze::bridgeUnvisitedCell(int curCell){
    //process navigatable directions (clockwise)
    bool directionsAvailable[4] = {
        (curCell - size >= 0),
        (curCell + 1) % size != 0,
        (curCell + size < size * size),
        (curCell % size != 0)
    };
    
    //choose one of the unvisited neighbours
    int dir, secNo;
    bool wallsTested[4] = { false, false, false, false };//walls verified if visited
    //treat the unavailable directions as tested walls
    for(int i = 0; i < 4; i++){
        if(!directionsAvailable[i])
            wallsTested[i] = true;
    }
    do{
        //check direction and test if its available
        dir = rand() % 4;
        while(!directionsAvailable[dir])
            dir = rand() % 4;
        
        if(wallsTested[0] && wallsTested[1] && wallsTested[2] && wallsTested[3])
            return -1;
        switch(dir){
            case 0:
                secNo = curCell - size;
                break;
            case 1:
                secNo = curCell + 1;
                break;
            case 2:
                secNo = curCell + size;
                break;
            case 3:
                secNo = curCell - 1;
                break;
        }
        wallsTested[dir] = true;
    } while(count(visited.begin(), visited.end(), secNo));//check if the neighbour is visited
    
    //remove the wall between the current cell and the chosen cell
    switch(dir){
        case 0:
            maze[secNo].setWallVisible(2);
            maze[curCell].setWallVisible(0);
            break;
        case 1:
            maze[secNo].setWallVisible(3);
            maze[curCell].setWallVisible(1);
            break;
        case 2:
            maze[secNo].setWallVisible(0);
            maze[curCell].setWallVisible(2);
            break;
        case 3:
            maze[secNo].setWallVisible(1);
            maze[curCell].setWallVisible(3);
            break;
    }
    
    return secNo;
}


//Instantiate maze scene, and generate maze.
void Maze::load(){
    maze[0].setWallVisible(3);
    maze[maze.size() - 1].setWallVisible(1);
    
    srand(time(NULL));
    stack.clear();
    visited.clear();
    stack.push_back(0);
    visited.push_back(0);
    
    //generates maze using the recursive implementation
    do{
        int neighbour = bridgeUnvisitedCell(stack[stack.size() - 1]);
        
        //invoke the routine recursively for a chosen cell
        if(neighbour == -1)
            stack.pop_back();
        else{
            stack.push_back(neighbour);
            visited.push_back(neighbour);
        }
    } while(stack[stack.size() - 1] != 0);
}

void Maze::print(){
    cout << " ";
    for(int c = 0; c < this->size; c++)
        cout << "_ ";
    cout << endl << " ";
    
    for(int r = 0; r < this->size; r++){
        if(r != 0)
            cout << "|";
        for(int c = 0; c < this->size; c++)
            maze[this->size * r + c].print();
        cout << endl;
    }
}

MazeSector Maze::getSector(int row, int column)
{
    int index = column * size + row;
    
    if(index < 0 || index >= maze.size())
    {
        index = 0;
    }
    
    return maze[index];
}

int Maze::getSize() { return size; }
