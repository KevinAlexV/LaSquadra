//
//  Maze.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-07.
//

#ifndef Maze_hpp
#define Maze_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

class MazeSector {
    bool walls[4];
    
public:
    MazeSector();
    void setWallVisible(int, bool=false);
    bool getWallVisible(int);
    void print();
};

class Maze{
    vector<int> stack;
    vector<int> visited;
    int size;
    int bridgeUnvisitedCell(int);
    void load();
    
public:
    vector<MazeSector> maze;
    Maze(int);
    void print();
    MazeSector getSector(int row, int column);
    int getSize();
};

#endif /* Maze_hpp */
