#include "ball.h"

#define MAX_BALLS 100

ball *balls;
int N;

void load_balls(void);
void print_balls(void);

int main(int argc, char *argv[]) {
  if (argc != 2)
    puts("usage: balls N"), exit(1);

  N = atoi(argv[1]);
  load_balls();

  set_screen_size(500, 500);
  set_draw_function(print_balls);
  gflush();

  return 0;
}

void load_balls(void) {
  register int i;

  if (N > MAX_BALLS)
    N = MAX_BALLS;

  balls = malloc(sizeof(ball) * N);
  
  for (i = 0; i < N; i++) {
    *(balls+i) = random_ball();
  }
}

void print_balls(void) {
  register int i;

  usleep(50 * 1000);

  for (i = 0; i < N; i++) {
    ball *b = balls + i;

#if LOG
    printf("ball %d before - x, y: %f, %f - vx, vy: %f, %f \n", i, b->x, b->y, b->vx, b->vy);
#endif

    move_ball(b, 50);

#if LOG
    printf("ball %d after - x, y: %f, %f - vx, vy: %f, %f \n", i, b->x, b->y, b->vx, b->vy);
#endif

    print_ball(b);
  }
}
