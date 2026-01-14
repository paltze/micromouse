#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "maze.h"

void FloodfillInit();
void FloodfillCompute();

Dir FloodfillBestNeighbor(int x, int y, Dir prefer);

#endif
