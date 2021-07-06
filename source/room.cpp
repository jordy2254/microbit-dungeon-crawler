/**
 * File: main.cpp
 * Author: Jordan
 * Desc: Contains information about a room, currntly only the directions which can be traveled.
 */

#include "room.h" // header in local directory

Room::Room(){
    dirs[DIRUP] = 0;
    dirs[DIRDOWN] = 0;
    dirs[DIRLEFT] = 0;
    dirs[DIRRIGHT] = 0;
}

// craig.duffy@uwe.ac.uk
