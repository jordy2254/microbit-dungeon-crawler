/**
 * File: Monster.cpp
 * Author: Jordan
 * Desc: Contains all monster stats
 */

#include "monster.h" // header in local directory

Monster::Monster(){}

//setup the monster
Monster::Monster(short dam,short init, short win){
    damageValue = dam;
    winReduction = win;
    initSpeed = init;
}

//damage the monster
void Monster::damage(){
    health -= damageValue;
}


//return if the monster's health is less than 0 IE dead
bool Monster::isDead(){
    return health <= 0;
}
