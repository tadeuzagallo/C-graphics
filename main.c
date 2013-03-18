#include "graphics.h"

static int i = 0;

void print(void) {
  i++;
  if (i > 7) i %= 7;
  usleep(500000);

  if (i & 1)
    foreground_color(0, 255, 0),
    draw_rect(-.8f, -.8f, 1.6f, 1.6f);

  if (i & 2)
  foreground_color(255, 0, 0),
  draw_circle(0.f, 0.f, 0.8f);

  if (i & 4)
  foreground_color(0, 0, 255),
  draw_circle(.5f, .5f, .5f);
}

int main(void) {
  set_screen_size(640, 480);
  background_color(255, 255, 255);

  set_draw_function(print);

  gflush();
  return 0;
}
