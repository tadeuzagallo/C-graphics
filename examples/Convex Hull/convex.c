#include "convex.h"
#include <time.h>
#include <math.h>

#pragma mark - User defined macros 

#define MAX 26
#define MAXXY 19
#define MINXY 0
#define POINT_RADIUS 0.015f

#pragma mark - User defined types

typedef struct {
  float x, y;
  double ang;
  char letter;
} point;

#pragma mark - Private variables

static int N, R, P, S;
static point *points, *result;

#pragma mark - Private functions prototypes

static void get_min(point *points);
static void sort(point *points);
static double ang(point a);
static void swap(point *a, point *b);
static int ccw(point a, point b, point c);
static void print_point(point *p);
static float print_coord(float coord);

#pragma mark - Private functions

static void get_min(point *points) {
  register short i;
  int p_index;
  float min = MAXXY;
  float minx = MAXXY;

  for (i = 0; i < N; i++)
    if (points[i].y < min || (points[i].y == min && points[i].x < minx)) 
        min = points[i].y, minx = points[i].x, p_index = i;
  swap(points, points+p_index);
}

static void sort(point *points) {
 register int i, j;

  for (i = 1; i < N; i++) {
    for (j = i; j > 1; j--) {
      if (points[j].ang < points[j-1].ang || 
          (points[j].ang == points[j-1].ang && 
           (points[j].y < points[j-1].y ||
            (points[j].y == points[j-1].y && points[j].x < points[j-1].x))))
        swap(points+j, points+j-1);
      else
        break;
    }
  }
}

static double ang(point b) {
  point a = *points;
  double ang = atan((b.y - a.y) / (b.x - a.x));
  
  if (ang < 0 || (ang == 0 && b.x < a.x))
    ang += M_PI;

  return ang;
}

static void swap(point *a, point *b) {
  point x = *a;
  *a = *b;
  *b = x;
}

static int ccw(point a, point b, point c) {
  double area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

  if (area < 0) return -1;
  else if (area > 0) return 1;
  else return 0;
}

static void print_point(point *p) {
    draw_circle(
        print_coord(p->x),
        print_coord(p->y),
        POINT_RADIUS
        );
}

static float print_coord(float coord) {
  return (coord / 10.f) - 0.9f;
}

#pragma mark Public methods

void print_cartesian(void) {
  register int i;

  draw_line(-0.9f, 1.f, -0.9f, -1.f);
  draw_line(1.f, -0.9f, -1.f, -0.9f);
  
  for (i = 0; i < 19; i++) {
    draw_circle((float) i/10.f - 0.899f, -0.899f, 0.01f);
    draw_circle(-0.899f, (float) i/10.f - 0.899f, 0.01f);
  }
}

void load_points(int n) {
  register int i;

  if (n > MAX)
    n = MAX;

  srand(time(NULL));

  points = malloc(sizeof(point) * n);
  N = n;

  for (i = 0; i < n; i++) {
    point p;
    p.x = MINXY + (rand() % MAXXY - MINXY);
    p.y = MINXY + (rand() % MAXXY - MINXY);
    p.letter = 'A' + i;
    *(points + i) = p;
  }

  get_min(points);

  for (i = 1; i < N; i++) {
    points[i].ang = ang(points[i]);
  }

  sort(points);
}

void print_points(void) {
  register int i;

  foreground_color(0, 0, 200);

  for (i = 0; i < N; i++) {
    point *p = points+i;
    //printf("printing point %d at %f, %f\n", i, p->x, p->y);
    print_point(p);
  }
}
void create_path(void) {
  register int i;
  int k1, k2;
  result = malloc(sizeof(point) * N);
  point *r = result;

  *r++ = *points, R++;

  for (k1 = 1; k1 < N; k1++) {
    if (points[k1].x != points->x || points[k1].y != points->y) 
      break;
  }

  if (k1 == N) return;

  for (k2 = k1 + 1; k2 < N; k2++) {
    if (ccw(*points, points[k1], points[k2])) break;
  }

  *r = points[k2 - 1], R++;
  
  for (i = k2; i < N; i++) {
    point top = *r--;
    R--;
    //printf("%d: %c %c %c %d\n", i, r->letter, top.letter, points[i].letter, ccw(*r, top, points[i]));
    while (ccw(*r, top, points[i]) <= 0) {
      top = *r--;
      R--;
    }

    *(++r) = top, R++;
    *(++r) = points[i], R++;

  }
}

void print_ordering(void) {
  point *pp = NULL;
  register int i;

  foreground_color(0, 0, 0);
  print_cartesian();

  foreground_color(0, 0, 200);
  print_points();

  for (i = 0; i <= P; i++) {
    point *p = points + i;

    print_point(p);
   
    if (pp) {
      draw_line(
          print_coord(points->x),
          print_coord(points->y), 
          print_coord(p->x),
          print_coord(p->y)
          );
    }

    pp = p;
  }


  if (P == N - 1) {
    set_draw_function(print_path);
  } else {
    P++;
    usleep(400 * 1000);
  }

}

void print_path(void) {
  point *pp = NULL;
  register int i;

  printf("printing point %d of path\n", S);

  print_ordering();

  S++;

  if (S > R) {
    result[R] = *result;
    set_draw_function(NULL);
  }

  foreground_color(255, 0, 0);

  for (i = 0; i < S; i++) {
    point *p = result + i;

    print_point(p);
   
    if (pp) {
      draw_line(
          print_coord(pp->x),
          print_coord(pp->y), 
          print_coord(p->x),
          print_coord(p->y)
          );
    }

    pp = p;
  }

  usleep(600 * 1000);
}
