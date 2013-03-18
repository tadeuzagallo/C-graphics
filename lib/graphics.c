#pragma mark - Imports

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
#include "graphics.h"

#pragma mark - Macros

#define INIT() if (!__init__) __init__ = 1, init();

#define ADD_SHAPE(TYPE, C) if (_s->TYPE) { \
  shape s; \
  s.next = _s; \
  _s = &s; \
} \
_s->TYPE = C;

#define ADD_CIRCLE(C) ADD_SHAPE(circle, C)

#define NEW_SHAPE() shape *s = malloc(sizeof(shape));\
  if (_s) _s->next = s;\
  else __s = s;\
  s->next = NULL;\
  s->shape.circle = NULL;\
  s->shape.rect = NULL;\
  _s = s;\

#define COLOR(R, G, B)  \
  { .r = ((float)R)/255.f, .g = ((float)G)/255.f, .b = ((float)B)/255.f }

#define FILL_COLOR(S, R, G, B)  \
  { S.r = ((float)R)/255.f, S.g = ((float)G)/255.f, S.b = ((float)B)/255.f ;}

#pragma mark - Custom types

typedef enum {
  TYPE_RECT = 1 << 0,
  TYPE_CIRCLE = 1 << 1,
  TYPE_LINE = 1 << 2
} type;

typedef struct {
  float r, g, b; 
} color;

typedef struct {
  float x, y, width, height;
  color c;
} rect;

typedef struct {
  float x, y, radius;
  color c;
} circle;

typedef struct {
  float from_x, from_y,
        to_x, to_y;
  color c;
} line;

union shapes {
    circle *circle;
    rect *rect;
    line *line;
};

typedef struct shape{
  union shapes shape;
  type t;
  struct shape *next;
} shape;

#pragma mark - Private global variables

static int __init__ = 0;
static int _screen_width = 400,
           _screen_height = 400;

static float _alpha = 1.f;

static color _background_color = COLOR(255, 255, 255),
             _foreground_color = COLOR(0, 0, 0);

static shape *_s = NULL, *__s;

void (*_draw_function)(void);

#pragma mark - Private function prototypes

static void init(void);
static void idle(void);
static void display(void);

#pragma mark - Public functions

void set_screen_size(int width, int height) {
  _screen_height = height;
  _screen_width = width;
}

void set_draw_function(void (*draw_function)(void)) {
  _draw_function = draw_function;
}

void background_color(int R, int G, int B) {
  FILL_COLOR(_background_color, R, G, B);
}

void foreground_color(int R, int G, int B) {
  FILL_COLOR(_foreground_color, R, G, B);
}

void draw_rect(float x, float y, float width, float height) {
  NEW_SHAPE();

  INIT();

  _s->t = TYPE_RECT;
  _s->shape.rect = malloc(sizeof(rect));
  _s->shape.rect->x = x;
  _s->shape.rect->y = y;
  _s->shape.rect->height = height;
  _s->shape.rect->width = width;
  _s->shape.rect->c= _foreground_color;
}

void draw_circle(float x, float y, float radius) {
  NEW_SHAPE();

  INIT();

  _s->t = TYPE_CIRCLE;
  _s->shape.circle = malloc(sizeof(circle));
  _s->shape.circle->x = x;
  _s->shape.circle->y = y;
  _s->shape.circle->radius = radius;
  _s->shape.circle->c = _foreground_color;
}

void draw_line(float from_x, float from_y, float to_x, float to_y) {
  NEW_SHAPE();

  INIT();

  _s->t = TYPE_LINE;
  _s->shape.line = malloc(sizeof(line));
  _s->shape.line->from_x = from_x;
  _s->shape.line->from_y = from_y;
  _s->shape.line->to_x = to_x;
  _s->shape.line->to_y = to_y;
  _s->shape.line->c = _foreground_color;
}

void gflush(void) {
  INIT();

  if (_draw_function)
    glutIdleFunc(idle);

  glutDisplayFunc(display);
  glutMainLoop();
}

#pragma mark - Private functions

static void init(void) {
  char *name = malloc(sizeof(char) * 9);
  int a = 1;

  strcpy(name, "graphics");

  glutInit(&a, &name);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(_screen_width, _screen_height);
  glutCreateWindow("");
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

static void idle(void) {
  glutPostRedisplay();
}

static void display(void) {
  shape *s = __s;

  if (_draw_function) _draw_function();

  glClearColor(_background_color.r, _background_color.g, _background_color.b, _alpha);
  glClear(GL_COLOR_BUFFER_BIT);

  while (s) {
    if (s->t & TYPE_CIRCLE) {
      int sections = 30, i;

      glColor3f(s->shape.circle->c.r, s->shape.circle->c.g, s->shape.circle->c.b);

      glBegin(GL_TRIANGLE_FAN);
        glVertex2f(s->shape.circle->x, s->shape.circle->y);

        for(i = 0; i <= sections;i++)
          glVertex2f(
              s->shape.circle->radius * cos(i *  2 * M_PI / sections) + s->shape.circle->x, 
              s->shape.circle->radius* sin(i * 2 * M_PI / sections) + s->shape.circle->y
              );

      glEnd();
    } else if (s->t & TYPE_RECT) {
      glColor3f(s->shape.rect->c.r, s->shape.rect->c.g, s->shape.rect->c.b);

      glBegin(GL_POLYGON);
        glVertex2f(s->shape.rect->x, s->shape.rect->y);
        glVertex2f(s->shape.rect->x + s->shape.rect->width, s->shape.rect->y);
        glVertex2f(s->shape.rect->x + s->shape.rect->width, s->shape.rect->y + s->shape.rect->height);
        glVertex2f(s->shape.rect->x, s->shape.rect->y + s->shape.rect->height);
      glEnd();
    } else if (s->t & TYPE_LINE) {
      glColor3f(s->shape.line->c.r, s->shape.line->c.g, s->shape.line->c.b);

      glBegin(GL_LINES);
        glVertex2f(s->shape.line->from_x, s->shape.line->from_y);
        glVertex2f(s->shape.line->to_x, s->shape.line->to_y);
      glEnd();
    }

    _s = s->next;
    s = _s;
  }

  glutSwapBuffers();
  glFlush();
}
