#include <time.h>
#include "../../lib/graphics.h"

#define MULTICOLOR 1
#define LOG 1

typedef struct {
  float vx, vy,
         x ,  y,
         radius;

#if MULTICOLOR
  int r, g, b;
#endif

} ball;

ball random_ball(void);
void move_ball(ball *b, int t);
void print_ball(ball *b);
