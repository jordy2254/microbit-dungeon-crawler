/**
 * File: room.h
 * Author: Jordan
 * Desc: Header file for room
 */
#ifndef ROOM_H
#define ROOM_H

#include "monster.h"
#define DIRUP 0
#define DIRRIGHT 1
#define DIRDOWN 2
#define DIRLEFT 3

class Room
{
public:
    Room();

    bool hasMonster = false;
    short monsterID = -1;

    bool dirs[4] = {0,0,0,0};
private:

};

#endif
