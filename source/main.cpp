/**
 * File: main.cpp
 * Author: Jordan
 * Desc: Main game loops and management of button presses and controls.
 */

#include "MicroBit.h"
#include "maze.h"
#include "monster.h"

MicroBit uBit;

const uint8_t arrowArr[] {0, 0, 0, 0, 0,
                          0, 0, 0, 1, 0,
                          1, 1, 1, 1, 1,
                          0, 0, 0, 1, 0,
                          0, 0, 0, 0, 0, };

MicroBitImage arrow(5,5,arrowArr);

char currentdir = DIRUP;

bool buttonPressedA = false, buttonReleasedA = false;
bool buttonPressedB = false, buttonReleasedB = false;

void getDirection();
void aButtonPressed(MicroBitEvent e);
void aButtonReleased(MicroBitEvent e);
void bButtonPressed(MicroBitEvent e);
void bButtonReleased(MicroBitEvent e);
void drawArrow(uint8_t dir);
void fight(Monster m);
void drawHealthDots(uint8_t start, short end);
void navMaze();

//states for keeping track of where we are in the program and processing buttons
//in the process.
// detail about how these function, and how there implemented can be seen in the IOT-flowchart.pdf
#define MOVESTATE 0
#define ROOMSTATE 1
#define FIGHTSTATE 2
#define ENDSTATE 3


uint8_t currentState = ROOMSTATE;

Maze maze(&uBit);

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    maze = Maze(&uBit);


    //set up the interupts on the buttons A & B for pressed and released.
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN, aButtonPressed);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP, aButtonReleased);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_DOWN, bButtonPressed);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_UP, bButtonReleased);


    while(1) {

        if(maze.completed()) currentState = ENDSTATE;

        if(currentState == ROOMSTATE){
            //if we've pressed the A button and after 200ms
            //havn't released then start move state and then move if possible.
            if(buttonPressedA){
                uBit.sleep(200);
                if(!buttonReleasedA){
                    currentState = MOVESTATE;
                    getDirection();

                    //maze.move(); return if there is a monster in the room
                    bool res = maze.move(currentdir);

                    if(res == false) currentState = ROOMSTATE;
                    else currentState = FIGHTSTATE;
                }
                buttonPressedA = buttonReleasedA = false ;
            }

            //if we've pressed the B button and after 200ms
            //havn't released then display location to user.
            if(buttonPressedB){
                uBit.sleep(200);
                if(!buttonReleasedB){
                    maze.printLoc();

                    //this is a function that displays the whole maze and scrolls it with the accleerometer,
                    //it was a cool idea but wasn't very easy to read. however the code still exists and can be used if wanted
                    //rather than the print loc.
                    // navMaze();
                }
                buttonPressedB = buttonReleasedB = false ;
            }
        }

        if(currentState == ROOMSTATE) maze.dispCurrentRoom();

        //if we've completed the maze then display winner until user resets or turns of microbit.
        if(currentState == ENDSTATE){
            uBit.display.scroll("Winner!");
            uBit.sleep(600);
        }
        //if we're in the fightstate
        //start the fight sequence and display the fight to user
        if(currentState == FIGHTSTATE){
            uBit.display.print("FIGHT!");
            uBit.sleep(500);
            fight(maze.getMonster());
            currentState = ROOMSTATE;
        }
        uBit.sleep(100);
    }
}

//fight sequence.
//the fight sequence is a simple minigame
//the code below is to simply align an LED on the bottom with the moving once
void fight(Monster m){

    //Vars for the minigame
    bool flip;
    short cx = 0, speed = m.initSpeed, tx = 3 - (uBit.random(3));

    //until the monster is dead.
    while(!m.isDead()){
        //clear the display and draw
        uBit.display.clear();

        //render game and target
        short prevDots = m.health / 10;

        //if there's any dot's to draw.
        if(prevDots > 0)drawHealthDots(0, prevDots);

        //draw the 2 game pixels
        uBit.display.image.setPixelValue(tx, 4, 255);
        uBit.display.image.setPixelValue(cx, 3, 255);

        //this resets the button values, to ensure the buttons have been pressed in the sleep section
        buttonPressedB = buttonReleasedB = false;
        uBit.sleep(speed);

        //if we've pressed the buttons and it matches do damage and flash health bar where damage is.
        if(buttonPressedB && buttonReleasedB && cx == tx){
            m.damage();
            short newDots = m.health / 10;

            speed -= m.winReduction;
            short i = 0;

            //this code flashes the health
            while(i < 6){
                //clear screen and draw static elements
                uBit.display.clear();
                uBit.display.image.setPixelValue(tx, 4, 255);
                uBit.display.image.setPixelValue(cx, 3, 255);

                if(newDots > 0) drawHealthDots(0, newDots);

                //every other "frame" draw the damage dots
                if(i %2 == 0){
                    if(prevDots - newDots > 0) drawHealthDots(newDots, prevDots);
                }
                i++;
                uBit.sleep(300);
            }

            //reset the button press.
            buttonPressedB = buttonReleasedB = false;
            //report the target.
            tx = 3 - (uBit.random(3));
        }
        //moves the current location
        if(flip) cx--;
        else cx++;

        //check bounds and flip if out of bounds.
        if(cx >4){
            flip = true;
            cx = 4;
        }else if (cx < 0){
            flip = false;
            cx = 0;
        }
    }

    //remove the monster from the maze if it has been killed.
    if(m.isDead()){
        maze.removeMonster();
    }
}

//draws the taget pixels to the screen as in a 1d array ie: a value of 5 would be x:0, Y1
//the end pouint8_t works in the same way, this could be improved in the future. using modulus and division.
void drawHealthDots(uint8_t start, short end){
    for(uint8_t y = 0; y < 2; y++){
        for(uint8_t x = 0; x <= 4; x++){
            if(x + y * 4 >= start && x + y * 4 <= end){
                uBit.display.image.setPixelValue(x, y, 255);
            }
        }
    }
}

//this is a function that displays the whole maze and scrolls it with the accleerometer,
//it was a cool idea but wasn't very easy to read. however the code still exists and can be used if wanted

void navMaze(){
    int offX = 0, offY = 0;


    while(!buttonReleasedB){
        short dx = uBit.accelerometer.getX();
        short dy = uBit.accelerometer.getY();

        if (abs(dx) > abs(dy))  {
            if(dx < 0) {
                offX += 1;
            } else {
                offX -= 1;
            }
        } else {
            if(dy < 0) {
                offY += 1;
            } else {
                offY -= 1;
            }
        }

        maze.draw(offX, offY);
        uBit.sleep(500);
    }
    buttonPressedB = buttonReleasedB = false ;
}
//This function gets the direction to move for the Maze
//it sets the currentdir value to
void getDirection(){
    //loop until a button is released.
    while(!buttonReleasedA){
        short dx = uBit.accelerometer.getX();
        short dy = uBit.accelerometer.getY();

        if (abs(dx) > abs(dy))  {
            if(dx < 0) {
                currentdir = DIRLEFT;
            } else {
                currentdir = DIRRIGHT;
            }
        } else {
            if(dy < 0) {
                currentdir = DIRUP;
            } else {
                currentdir = DIRDOWN;
            }
        }
        drawArrow(currentdir);
        uBit.sleep(100);
    }
}

//this function draws the arrow for the controller.
//the function uses the one array above, and flips/rotates it to display correctly
void drawArrow(uint8_t dir){
    uBit.display.clear();
    for(uint8_t x = 0; x < 5; x++){
        for(uint8_t y = 0; y < 5; y ++){
            if(dir == DIRRIGHT) uBit.display.image.setPixelValue(x,y,arrowArr[x + y * 5]);
            if(dir == DIRLEFT) uBit.display.image.setPixelValue((4 - x),y,arrowArr[x + y * 5]);
            if(dir == DIRUP) uBit.display.image.setPixelValue(y,(4 - x),arrowArr[x + y * 5]);
            if(dir == DIRDOWN) uBit.display.image.setPixelValue(y,x,arrowArr[x + y * 5]);
        }
    }
}

//Button events to set values above, so we can detect if buttons have been pressed or released
void aButtonPressed(MicroBitEvent e){
    buttonPressedA = true;
}

void aButtonReleased(MicroBitEvent e){
    buttonReleasedA = true;
}

void bButtonPressed(MicroBitEvent e){
    buttonPressedB = true;
}

void bButtonReleased(MicroBitEvent e){
    buttonReleasedB = true;
}
