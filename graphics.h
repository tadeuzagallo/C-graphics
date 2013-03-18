#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void set_screen_size(int width, int height);
void set_draw_function(void (*)(void));
void background_color(int R, int G, int B);
void foreground_color(int R, int G, int B);
void draw_rect(float x, float y, float width, float height);
void draw_circle(float x, float y, float radius);
void draw_line(float from_x, float from_y, float to_x, float to_y);
void gflush(void);
