/**
 * File: Maze.cpp
 * Author: Jordan
 * Desc: Handles storing of all rooms and generation.
 */

#include "maze.h" // header in local directory
#include <cstring>

Room rooms[SIZE * SIZE];

//generates the maze and sets our reference to the microbit instance in main.
Maze::Maze(MicroBit *currentInstance){
    uBit = currentInstance;
    generate();
}

//Return true if we're at the end point and have defated all monsters
bool Maze::completed(){
    return (px == ex && py == ey) && (defeatedMonsters == MONSTERS);
}

//move function, checks bounds and direction of room and moves if within.
bool Maze::move(uint8_t dir){
    if(dir == DIRUP && rooms[px + py * SIZE].dirs[DIRUP] == 1){
        if(py > 0) py --;
    }else if(dir == DIRDOWN && rooms[px + py * SIZE].dirs[DIRDOWN] == 1){
        if(py < SIZE - 1) py ++;
    }else if(dir == DIRLEFT && rooms[px + py * SIZE].dirs[DIRLEFT] == 1){
        if(px > 0)px --;
    }else if(dir == DIRRIGHT && rooms[px + py * SIZE].dirs[DIRRIGHT] == 1){
        if(px < SIZE - 1)px ++;
    }
    //return if there's a monset
    return rooms[px + py * SIZE].hasMonster;
}


//pruint8_t the x and y location of the player.
void Maze::printLoc(){
    uBit->display.print("X: ");
    uBit->display.print(px);
    uBit->display.print("Y: ");
    uBit->display.print(py);
    uBit->sleep(MICROBIT_DEFAULT_PRINT_SPEED * 10);
}

//draw the curret room to the screen,
//end POuint8_t has a dot in the center;
void Maze::dispCurrentRoom(){
    uBit->display.clear();

    if(px == ex && py == ey) uBit->display.image.setPixelValue(2,2,255);

    drawWall(rooms[px + py * SIZE].dirs[DIRUP] == 1, DIRUP);
    drawWall(rooms[px + py * SIZE].dirs[DIRDOWN] == 1, DIRDOWN);
    drawWall(rooms[px + py * SIZE].dirs[DIRLEFT] == 1, DIRLEFT);
    drawWall(rooms[px + py * SIZE].dirs[DIRRIGHT] == 1, DIRRIGHT);
}

//creates a monster from the given paremeters within the room
Monster Maze::getMonster(){
    uint8_t id = rooms[px + py * SIZE].monsterID;
    Monster m(monsters[id][0], monsters[id][1], monsters[id][2]);
    return m;
}

//remove the monster from the room, and incrememnet defeatedMonsters
void Maze::removeMonster(){
    defeatedMonsters += 1;
    rooms[px + py * SIZE].hasMonster = 0;
}


//this draws a smaller version of the maze with each room taking up a 3x3 meaning that we can fit 3 rooms on the 5x5 by overlapping the walls.
void Maze::draw(short offX, short offY){
    uBit->display.clear();
    for(int x = 0; x < SIZE; x++){
        for(int y = 0; y < SIZE; y++){
            drawWallSmall(rooms[x + y * SIZE].dirs[DIRUP] == 1, DIRUP,offX + (2 * x),offY + (2 * y));
            drawWallSmall(rooms[x + y * SIZE].dirs[DIRDOWN] == 1, DIRDOWN,offX + (2 * x),offY + (2 * y));
            drawWallSmall(rooms[x + y * SIZE].dirs[DIRLEFT] == 1, DIRLEFT,offX + (2 * x),offY + (2 * y));
            drawWallSmall(rooms[x + y * SIZE].dirs[DIRRIGHT] == 1, DIRRIGHT,offX + (2 * x),offY + (2 * y));
        }
    }
}

//sets the pixel values on the screen at the offset acroding to the size
//used for drawing the nav maze section.
void Maze::drawWallSmall(bool open, uint8_t side, int offX, int offY){
    if(side == DIRUP){
        for(uint8_t x = 0; x < 3; x++){
            if (open) uBit->display.image.setPixelValue(x + offX,0 + offY,roomOpenSmall[x]);
            else uBit->display.image.setPixelValue(x + offX,0 + offY,roomCloseSmall[x]);
        }
    }else if(side == DIRDOWN){
        for(uint8_t x = 0; x < 3; x++){
            if (open) uBit->display.image.setPixelValue(x + offX,4 + offY,roomOpenSmall[x]);
            else uBit->display.image.setPixelValue(x + offX,4 + offY,roomCloseSmall[x]);
        }
    }else if(side == DIRLEFT){
        for(uint8_t y = 0; y < 3; y++){
            if (open) uBit->display.image.setPixelValue(0 + offX,y + offY,roomOpenSmall[y]);
            else uBit->display.image.setPixelValue(0 + offX,y + offY,roomCloseSmall[y]);
        }
    }else if(side == DIRRIGHT){
        for(uint8_t y = 0; y < 3; y++){
            if (open) uBit->display.image.setPixelValue(4 + offX,y + offY,roomOpenSmall[y]);
            else uBit->display.image.setPixelValue(4 + offX,y + offY,roomCloseSmall[y]);
        }
    }
}
//draws a wall on one side using the arrays in the maze.h
void Maze::drawWall(bool open, uint8_t side){
    if(side == DIRUP){
        for(uint8_t x = 0; x < 5; x++){
            if (open) uBit->display.image.setPixelValue(x,0,roomOpen[x]);
            else uBit->display.image.setPixelValue(x,0,roomClose[x]);
        }
    }else if(side == DIRDOWN){
        for(uint8_t x = 0; x < 5; x++){
            if (open) uBit->display.image.setPixelValue(x,4,roomOpen[x]);
            else uBit->display.image.setPixelValue(x,4,roomClose[x]);
        }
    }else if(side == DIRLEFT){
        for(uint8_t y = 0; y < 5; y++){
            if (open) uBit->display.image.setPixelValue(0,y,roomOpen[y]);
            else uBit->display.image.setPixelValue(0,y,roomClose[y]);
        }
    }else if(side == DIRRIGHT){
        for(uint8_t y = 0; y < 5; y++){
            if (open) uBit->display.image.setPixelValue(4,y,roomOpen[y]);
            else uBit->display.image.setPixelValue(4,y,roomClose[y]);
        }
    }
}




/*generation algorithm for the maze:
    A visual representation can be seen with the mazegen.jar which features the same algorithm just written in java
    detail can be seen within the README.md

    uses a recursive backtracking algorithm:
    details found here: https://en.wikipedia.org/wiki/Maze_generation_algorithm
    PSUEDO CODE:
    get the children current location
    if the children size == 0
     set the current location to the next element off the unvisStack
     repeat above
    if the children size - 1 > 0
     add the currentlocation to the unvis unvisStack
    end if

    get a random child and that will be the location we're going
     remove the wall from the random child to the currentPoint
    set the current point to the child and loop

    loop above until the stack is empty or we've search the whole maze
*/
void Maze::generate(){

    checked.clear();
    uBit->sleep(100);
    //Monsters
    //Self Damage | init speed | win Reduction | PLayer damage
    setMonsterStats(0, new short[4]{25, 450, 35});
    setMonsterStats(1, new short[4]{20, 400, 30});

    //Monster addition
    //0,3 has monster 0
    rooms[0 + 1 * SIZE].hasMonster = 1;
    rooms[0 + 1 * SIZE].monsterID = 0;

    //9,9 has monster 4
    rooms[9 + 9 * SIZE].hasMonster = 1;
    rooms[9 + 9 * SIZE].monsterID = 1;

    //generation algorithm.

    Point currentPoint(0,0);

    std::vector<Point> unvisStack;

    while(checked.size() < SIZE * SIZE){
        if(checkPoint(currentPoint)) checked.push_back(currentPoint);

        //generate all possible end point (children)
        std::vector<Point> children;
        while(children.size() == 0){

            //generate all possible children
            if(checkPoint(Point(currentPoint.x - 1, currentPoint.y)) && currentPoint.x > 0) {
                children.push_back(Point(currentPoint.x - 1, currentPoint.y));
            }

            if(checkPoint(Point(currentPoint.x + 1, currentPoint.y)) && currentPoint.x < SIZE - 1) {
                children.push_back(Point(currentPoint.x + 1, currentPoint.y));
            }

            if(checkPoint(Point(currentPoint.x, currentPoint.y - 1)) && currentPoint.y > 0) {
                children.push_back(Point(currentPoint.x, currentPoint.y - 1));
            }

            if(checkPoint(Point(currentPoint.x, currentPoint.y + 1)) && currentPoint.y < SIZE - 1) {
                children.push_back(Point(currentPoint.x, currentPoint.y + 1));
            }

            //check the size of the children and do the appropriate action
            if(children.size() == 0){
                if(unvisStack.size() == 0) break;

                currentPoint = unvisStack.at(unvisStack.size() - 1);
                unvisStack.pop_back();
            }
        }

        if(children.size() == 0) break;

        //select a random direction to travel.
        int child = uBit->random(children.size());
        Point endPoint = children.at(child);

        if(children.size() - 1 > 0) unvisStack.push_back(currentPoint);


        //calculate the end point and remove the walls.
        if(endPoint.x < currentPoint.x){
            rooms[endPoint.x + endPoint.y * SIZE].dirs[DIRRIGHT] = 1;
            rooms[currentPoint.x + currentPoint.y * SIZE].dirs[DIRLEFT] = 1;
        }else if(endPoint.x > currentPoint.x){
            rooms[endPoint.x + endPoint.y * SIZE].dirs[DIRLEFT] = 1;
            rooms[currentPoint.x + currentPoint.y * SIZE].dirs[DIRRIGHT] = 1;
        }else if(endPoint.y < currentPoint.y){
            rooms[endPoint.x + endPoint.y * SIZE].dirs[DIRDOWN] = 1;
            rooms[currentPoint.x + currentPoint.y * SIZE].dirs[DIRUP] = 1;
        }else if(endPoint.y > currentPoint.y){
            rooms[endPoint.x + endPoint.y * SIZE].dirs[DIRUP] = 1;
            rooms[currentPoint.x + currentPoint.y * SIZE].dirs[DIRDOWN] = 1;
        }

        currentPoint = endPoint;

        if(unvisStack.size() == 0) break;
    }

    //removes some random walls from the maze, to make the next maze less row like and add some extra randomisation
    //walls are added at an 85%(ish) chance if the room only has 2 possible direction
    for(int x = 0; x < SIZE; x++) {
        for(int y = 0; y < SIZE; y++) {
            std::vector<int> dirs;

            if(rooms[x + y * SIZE].dirs[DIRUP] == 0 && y > 0) {
                dirs.push_back(DIRUP);
            }
            if(rooms[x + y * SIZE].dirs[DIRDOWN] == 0 && y < SIZE - 1) {
                dirs.push_back(DIRDOWN);
            }
            if(rooms[x + y * SIZE].dirs[DIRLEFT] == 0 && x > 0) {
                dirs.push_back(DIRLEFT);
            }
            if(rooms[x + y * SIZE].dirs[DIRRIGHT] == 0 && x < SIZE -1) {
                dirs.push_back(DIRRIGHT);
            }

            if(dirs.size() == 2 && uBit->random(100) >= 85) {
                int dir = dirs.at(uBit->random(dirs.size()));
                rooms[x + y * SIZE].dirs[dir] = 1;
            }
        }
    }


    //ensure all directions are 2 way.
    for(uint8_t x = 0; x < SIZE; x++) {
        for(uint8_t y = 0; y < SIZE; y++) {
            if(rooms[x + y * SIZE].dirs[DIRUP] == 1) {
                rooms[x + (y - 1) * SIZE].dirs[DIRDOWN] = 1;
            }
            if(rooms[x + y * SIZE].dirs[DIRDOWN] == 1) {
                rooms[x + (y + 1) * SIZE].dirs[DIRUP] = 1;
            }
            if(rooms[x + y * SIZE].dirs[DIRLEFT] == 1) {
                rooms[(x - 1) + y * SIZE].dirs[DIRRIGHT] = 1;
            }
            if(rooms[x + y * SIZE].dirs[DIRRIGHT] == 1) {
                rooms[(x + 1) + y * SIZE].dirs[DIRLEFT] = 1;
            }
        }
    }
}

//return false if the point has been checked.
//true if the point doesn't exist in checked.
bool Maze::checkPoint(Point point){
    for(uint8_t i = 0; i < checked.size(); i++){
        Point tmp = checked.at(i);
        if(tmp.x == point.x && tmp.y == point.y){
            return false;
        }
    }
    return true;
}

//automatically populates the monsters array with the given values.
void Maze::setMonsterStats(short id, short values[3]){
    for(uint8_t i = 0; i < 3; i ++){
        monsters[id][i] = values[i];
    }
}
