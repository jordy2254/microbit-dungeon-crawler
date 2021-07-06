# Dungeon crawler Game for microbit
This project is a fully functioning highly detailed Dungeon crawler game made for the Microbit. It's been created in C++ 
Using Lancaster Universities Microbit runtime. Featuring a random maze generation algorithm, monster fighting system and win state.

> ***IMPORTANT*** There are currently no images or videos of this project, this is something I hope to add in the coming 
> months as I work through my various projects.

## Key Learning outcomes
Embedded systems development poses it's own set of challenges including the limited processing power, Flash Memory and RAM. 
This project taught me alot about consideration of design when working with these restrictions. It highlighted where a range of
ineffciencies can come in and ensured proper memory management throughout. 

## Installation
In order to install the project you will need a Microbit and a copy of the Lancaster univeristy Microbit runtime setup 
[instructions here](https://lancaster-university.github.io/microbit-docs/offline-toolchains/). Alternativly I have also commited the compiled
application which is called `dungeon_crawler.hex` and is located in the main folder of this repository. (This has been done to make it easier
trying the application due to the complexity of Microbit runtime setup)

> ***DISCLAIMER*** I take no responsibility for the pre-compiled application although every care has been taken to ensure it's correct
> there is still potential room for error's so please use at your own risk. Any issues with compiling please feel free to contact me
## File breakdown
- Main.cpp
  This file contains the main entry point of the program, It handles the starting of all loops and calls the appropriate functions to make the game work. it also contains the fight system too.
- maze.h & maze.cpp
  the maze file is in charge of holding all maze information, and player information. it contains functions for moving and drawing the maze to the microbits screen, it also contains an array of all monsters within the game.
- monster.h & monster.cpp
  this file contains the information for the monster, such as health damage etc.
- room.h & room.cpp
  this file contains all the information for a specific room, such a monsterID if it has a monster and more.

#Documentation
Within some comments is relations to documents, these are located within the DOCUMENTATION folder.

- IOT-flowchart.pdf: this file is a flowchart for the whole program and displays the different states and what each one does, provding information ontop of the comments.

### The Game
The game itself is a maze using the limited 5x5 monochrome pixels. because of this only one room can be displayed. this makes the maze harder in comparision to a computer where you can see it all, and also means a smaller maze is more accaptable.

- The end point is always 9,9 (this can be changed in the configuration if making another maze)
- there are always 2 monsters one at 1,1 and one at 9,9. Both of these need to be defeated to win the maze.
- Maze layout is always a 10x10 randomly generated maze

### Controls

The controls for the game are easy and are as follows:

- ROOM STATE Controls
  - Hold A button and tilt to move in direction of the arrow on screen (uses the accelerometer), the areas where there is no lit LED's is the directon you can go.
  - Hold B to show the whole maze
    - tilt the Microbit in the direction to move.
    - the rendered room size moves to 3, so 4 rooms can be displayed on the Microbit.
- FIGHT STATE controls
  - Around the maze is a series of monsters, the idea of the game is to align the 2 LED's on the bottom of the screen until they are dead (health is displayed on the top 2 rows with each LED representing approx 10% however the design will show 12% as 2 LEDS)
  - Push the B button to stop the LED, the button CANNOT be held then released as the code prevents this. it needs to be pushed at the correct time.

## Maze generator:
A Visualisation of the maze generation has been created within a java application. This was done to aid understanding when creating
the project and to prevent potential bugs. Porting to the Microbit created its own set of challenges with flash memory and RAM
constraints. The repository for the Java maze generator can be found [Here](https://github.com/jordy2254/java_maze_generator) and features a multithreaded UI which creates a 
visualisation of how the algorithm works.