#include "floodfill.h"

uint8_t min(uint8_t a, uint8_t b) {
  return (a > b) ? b : a;
}

void FloodfillInit() {
  for (int y = 0; y < MAZE_SIZE; y++)
    for (int x = 0; x < MAZE_SIZE; x++)
      MazeSetDist(x, y, 255);

  int c = MAZE_SIZE / 2;
  MazeSetDist(c, c, 0);
  MazeSetDist(c-1, c, 0);
  MazeSetDist(c, c-1, 0);
  MazeSetDist(c-1, c-1, 0);
}

void FloodfillCompute() {
  bool changed;
  do {
    changed = false;
    for (int y = 0; y < MAZE_SIZE; y++) {
      for (int x = 0; x < MAZE_SIZE; x++) {
        uint8_t best = MazeGetDist(x, y);

        if (!MazeHasWall(x,y,NORTH)) best = min(best, (uint8_t)(MazeGetDist(x, y+1) + 1));
        if (!MazeHasWall(x,y,EAST))  best = min(best, (uint8_t)(MazeGetDist(x+1, y) + 1));
        if (!MazeHasWall(x,y,SOUTH)) best = min(best, (uint8_t)(MazeGetDist(x, y-1) + 1));
        if (!MazeHasWall(x,y,WEST))  best = min(best, (uint8_t)(MazeGetDist(x-1, y) + 1));

        if (best < MazeGetDist(x, y)) {
          MazeSetDist(x, y, best);
          changed = true;
        }
      }
    }
  } while (changed);
}

Dir FloodfillBestNeighbor(int x, int y, Dir prefer) {
  Dir order[4] = {
    prefer,
    (Dir)((prefer + 3) % 4),
    (Dir)((prefer + 1) % 4),
    (Dir)((prefer + 2) % 4)
  };

  uint8_t best = 255;
  Dir bestDir = prefer;

  for (int i = 0; i < 4; i++) {
    Dir d = order[i];
    if (MazeHasWall(x, y, d)) continue;

    int nx = x, ny = y;
    if (d == NORTH) ny++;
    if (d == SOUTH) ny--;
    if (d == EAST)  nx++;
    if (d == WEST)  nx--;

    uint8_t dist = MazeGetDist(nx, ny);
    if (dist < best) {
      best = dist;
      bestDir = d;
    }
  }

  return bestDir;
}
