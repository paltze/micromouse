#include "maze.h"
#include <EEPROM.h>

#define EEPROM_MAGIC 0xA5

#define WALL_N (1 << 0)
#define WALL_E (1 << 1)
#define WALL_S (1 << 2)
#define WALL_W (1 << 3)

struct Cell {
  uint8_t walls;
  uint8_t dist;
};

static Cell maze[MAZE_SIZE][MAZE_SIZE];

static bool inBounds(int x, int y) {
  return x >= 0 && y >= 0 && x < MAZE_SIZE && y < MAZE_SIZE;
}

void MazeClear() {
  for (int y = 0; y < MAZE_SIZE; y++)
    for (int x = 0; x < MAZE_SIZE; x++) {
      maze[y][x].walls = 0;
      maze[y][x].dist  = 255;
    }
}

void MazeInit() {
  MazeClear();
}

void MazeSetWall(int x, int y, Dir d) {
  if (!inBounds(x, y)) return;

  maze[y][x].walls |= (1 << d);

  int nx = x, ny = y;
  if (d == NORTH) ny++;
  if (d == SOUTH) ny--;
  if (d == EAST)  nx++;
  if (d == WEST)  nx--;

  if (!inBounds(nx, ny)) return;

  maze[ny][nx].walls |= (1 << ((d + 2) % 4));
}

bool MazeHasWall(int x, int y, Dir d) {
  if (!inBounds(x, y)) return true;
  return maze[y][x].walls & (1 << d);
}

uint8_t MazeGetDist(int x, int y) {
  if (!inBounds(x, y)) return 255;
  return maze[y][x].dist;
}

void MazeSetDist(int x, int y, uint8_t d) {
  if (!inBounds(x, y)) return;
  maze[y][x].dist = d;
}

bool MazeLoadFromFlash() {
  if (EEPROM.read(0) != EEPROM_MAGIC)
    return false;

  int addr = 1;
  for (int y = 0; y < MAZE_SIZE; y++)
    for (int x = 0; x < MAZE_SIZE; x++)
      maze[y][x].walls = EEPROM.read(addr++);

  return true;
}

void MazeSaveToFlash() {
  EEPROM.write(0, EEPROM_MAGIC);

  int addr = 1;
  for (int y = 0; y < MAZE_SIZE; y++)
    for (int x = 0; x < MAZE_SIZE; x++)
      EEPROM.write(addr++, maze[y][x].walls);

  EEPROM.commit();
}
