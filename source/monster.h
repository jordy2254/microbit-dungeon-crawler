/**
 * File: monster.h
 * Author: Jordan
 * Desc: Header file for Monster
 */
#ifndef MONSTER_H
#define MONSTER_H


class Monster
{
public:
    short health = 100;       //monster health
    short winReduction = 53;  //Speed reduction for minigame
    short initSpeed = 450;    //init speed for minigame
    short damageValue = 35;   //damage to monster

    Monster();
    Monster(short dam,short init, short win);
    void damage();
    bool isDead();
private:

};

#endif
