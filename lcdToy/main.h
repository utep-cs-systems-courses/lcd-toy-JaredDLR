#ifndef main_included
#define main_included

#define BALL_RADIUS 2
#define BAR_WIDTH 16
#define BAR_HEIGHT 2
#define BAR_SPEED 8

typedef struct {
  unsigned char x;
  unsigned char y;
} Vector;

typedef struct {
  Vector drawPosition;
  Vector controlPosition;
  Vector velocity;
} Ball;

typedef struct {
  unsigned char score;
  Vector drawPosition;
  Vector controlPosition;
} Bar;

extern char switches;

#endif
