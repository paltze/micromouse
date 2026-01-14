#ifndef MAZE_H
#define MAZE_H

#include <stdint.h>
#include <stdbool.h>

#define MAZE_SIZE 16

enum Dir { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

void MazeInit();
void MazeClear();

void MazeSetWall(int x, int y, Dir d);
bool MazeHasWall(int x, int y, Dir d);

uint8_t MazeGetDist(int x, int y);
void MazeSetDist(int x, int y, uint8_t d);

bool MazeLoadFromFlash();
void MazeSaveToFlash();

#endif
