#include "state.h"
#include "maze.h"
#include "floodfill.h"
#include "motion.h"
#include "ir_sensor.h"
#include "motor.h"

static int mouseX = 0;
static int mouseY = 0;
static Dir mouseDir = NORTH;

enum Mode { EXPLORE, SPEEDRUN };
static Mode mode;

static Dir leftOf(Dir d)  { return (Dir)((d + 3) % 4); }
static Dir rightOf(Dir d) { return (Dir)((d + 1) % 4); }
static Dir backOf(Dir d)  { return (Dir)((d + 2) % 4); }

static void updateWallsAtCurrentCell() {
  int x = mouseX;
  int y = mouseY;
  Dir d = mouseDir;

  // Front wall: both front sensors agree
  if (IRSensorIsWall(FRONT_LEFT) || IRSensorIsWall(FRONT_RIGHT)) {
    MazeSetWall(x, y, d);
  }

  // Left wall
  if (IRSensorIsWall(FRONT_LEFT_FAR)) {
    MazeSetWall(x, y, leftOf(d));
  }

  // Right wall
  if (IRSensorIsWall(FRONT_RIGHT_FAR)) {
    MazeSetWall(x, y, rightOf(d));
  }

  // Back wall (optional, but nice for consistency)
  if (IRSensorIsWall(REAR)) {
    MazeSetWall(x, y, backOf(d));
  }
}

static void rotateTo(Dir from, Dir to) {
  int diff = (to - from + 4) % 4;

  if (diff == 0) return;

  float angle = 0.0f;

  if (diff == 1) angle = 90.0f;    // right
  if (diff == 3) angle = -90.0f;   // left
  if (diff == 2) angle = 180.0f;   // U-turn

  RotationTracker rt(angle);
  rt.init();

  while (!rt.done()) {
    float corr = rt.tick();
    if (angle > 0)
      turnRight(corr);
    else
      turnLeft(corr);
  }

  brakeMotion();
}

static void moveOneCell(Dir targetDir) {
  // 1. rotate to target heading
  rotateTo(mouseDir, targetDir);
  mouseDir = targetDir;

  // 2. move forward one cell
  DistanceTracker dt(10, 30);
  dt.init();

  while (!dt.done()) {
    float corr = dt.tick();   // already includes PD_straight
    moveForward(corr);
  }

  brakeMotion();

  // 3. update logical position
  if (mouseDir == NORTH) mouseY++;
  if (mouseDir == SOUTH) mouseY--;
  if (mouseDir == EAST)  mouseX++;
  if (mouseDir == WEST)  mouseX--;
}

void StateSetup() {
  MazeInit();

  if (MazeLoadFromFlash()) {
    mode = SPEEDRUN;
    FloodfillInit();
    FloodfillCompute();
  } else {
    mode = EXPLORE;
    FloodfillInit();
  }
}

void StateLoop() {
  IRSensorUpdate();

  if (mode == EXPLORE) {
    updateWallsAtCurrentCell();
    FloodfillCompute();
  }

  if (MazeGetDist(mouseX, mouseY) == 0) {
    if (mode == EXPLORE) MazeSaveToFlash();
    brakeMotion();
    while (1);
  }

  Dir next = FloodfillBestNeighbor(mouseX, mouseY, mouseDir);
  moveOneCell(next);   // your existing motion logic
}
