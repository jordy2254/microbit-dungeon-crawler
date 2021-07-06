/**
 * File: maze.h
 * Author: Jordan
 * Desc: Header file for maze
 */
#ifndef MAZE_H
#define MAZE_H

#include "MicroBit.h"
#include "room.h"

#include <vector>


#define SIZE 10
#define MONSTERS 2
class Maze
{
public:
    Maze(MicroBit *currentInstance);
    bool move(uint8_t dir);
    void dispCurrentRoom();
    void draw(short offX, short offY);


    void printLoc();
    void generate();
    void removeMonster();
    Monster getMonster();
    bool completed();

    void setMonsterStats(short id, short values[3]);

    struct Point{
        uint8_t x;
        uint8_t y;

        Point(uint8_t x2, uint8_t y2){
            x = x2;
            y = y2;
        }
    };
    std::vector<Point> checked;

    bool checkPoint(Point point);

    uint8_t px = 0, py = 0;
    uint8_t ex = SIZE - 1, ey = SIZE - 1;

    uint8_t defeatedMonsters = 0;


    short monsters[MONSTERS][3];

    uint8_t roomClose[5] = {1,1,1,1,1};
    uint8_t roomOpen[5] = {1,0,0,0,1};

    uint8_t roomCloseSmall[3] = {1,1,1};
    uint8_t roomOpenSmall[3] = {1,0,1};

    void drawWall(bool open, uint8_t side);

    void drawWallSmall(bool open, uint8_t side, int offX, int offY);
    MicroBit *uBit;

private:

};


#endif
