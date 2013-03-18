#include "ball.h"

#define MAX(A, B) (A > B ? A : B)
#define MIN(A, B) (A < B ? A : B)

void init(void) {
  static int _init;

  if (!_init) {
    _init = 1;
    srand((unsigned int) time(NULL));
  }
}

ball random_ball(void) {
  ball b;

  b.radius = 0.02f; //(float) (rand() % 1000) / 10000.f;
  b.vx = (float) ((rand() % 2000) - 1000) / 1000000.f;
  b.vy = (float) ((rand() % 2000) - 1000) / 1000000.f;
  b.x = (float) ((rand() % 2000) - 1000) / 1000.f;
  b.y = (float) ((rand() % 2000) - 1000) / 1000.f;

  b.x = MIN(MAX(b.x, b.radius - 1.f), 1.f - b.radius);
  b.y = MIN(MAX(b.y, b.radius - 1.f), 1.f - b.radius);

#if MULTICOLOR
  b.r = rand() % 255;
  b.g = rand() % 255;
  b.b = rand() % 255;
#endif

#if LOG
  printf("Initializing ball\n\
    x,   y: %f, %f\n\
    vx, vy: %f, %f\n\
    radius: %f\n",
    b.x, b.y, b.vx, b.vy, b.radius);
#endif

  return b;
}

void move_ball(ball *b, int t) {
  if ((b->x + b->vx * t < -1.f + b->radius) || (b->x + b->vx * t > 1.0 - b->radius))
    b->vx = -b->vx;
  
  if ((b->y + b->vy * t < -1.f + b->radius) || (b->y + b->vy * t > 1.0 - b->radius))
    b->vy = -b->vy;

  b->x += b->vx * t;
  b->y += b->vy * t;
}

void print_ball(ball *b) {
#if MULTICOLOR 
  foreground_color(b->r, b->g, b->b);
#endif

  draw_circle(b->x, b->y, b->radius);
}
